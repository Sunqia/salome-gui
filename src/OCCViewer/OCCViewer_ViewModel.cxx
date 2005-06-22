
#include "OCCViewer_ViewModel.h"
#include "OCCViewer_ViewWindow.h"
#include "OCCViewer_VService.h"
#include "OCCViewer_ViewPort3d.h"

#include "SUIT_ViewWindow.h"
#include "SUIT_Desktop.h"
#include "SUIT_Session.h"

#include <qpainter.h>
#include <qapplication.h>
#include <qcolordialog.h>
#include <qpalette.h>
#include <qpopupmenu.h>

#include <AIS_Axis.hxx>
#include <AIS_Drawer.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <Geom_Axis2Placement.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_LineAspect.hxx>

OCCViewer_Viewer::OCCViewer_Viewer( bool DisplayTrihedron )
: SUIT_ViewModel(),
myBgColor( Qt::black )
{
  // init CasCade viewers
  myV3dViewer = OCCViewer_VService::Viewer3d( "", (short*) "Viewer3d", "", 1000.,
                                              V3d_XposYnegZpos, true, true );

  myV3dViewer->Init();

  myV3dCollector = OCCViewer_VService::Viewer3d( "", (short*) "Collector3d", "", 1000.,
                                                 V3d_XposYnegZpos, true, true );
  myV3dCollector->Init();

    // init selector
  myAISContext = new AIS_InteractiveContext( myV3dViewer, myV3dCollector);

  clearViewAspects();

  /* create trihedron */
  if( DisplayTrihedron )
  {
    Handle(Geom_Axis2Placement) anAxis = new Geom_Axis2Placement(gp::XOY());
    myTrihedron = new AIS_Trihedron(anAxis);
    myTrihedron->SetInfiniteState( Standard_True );

    Quantity_Color Col(193/255., 205/255., 193/255., Quantity_TOC_RGB);
    //myTrihedron->SetColor( Col );
    myTrihedron->SetArrowColor( Col.Name() );
    myTrihedron->SetSize(100);
    Handle(AIS_Drawer) drawer = myTrihedron->Attributes();
    if (drawer->HasDatumAspect()) {
        Handle(Prs3d_DatumAspect) daspect = drawer->DatumAspect();
        daspect->FirstAxisAspect()->SetColor(Quantity_Color(1.0, 0.0, 0.0, Quantity_TOC_RGB));
        daspect->SecondAxisAspect()->SetColor(Quantity_Color(0.0, 1.0, 0.0, Quantity_TOC_RGB));
        daspect->ThirdAxisAspect()->SetColor(Quantity_Color(0.0, 0.0, 1.0, Quantity_TOC_RGB));
    }

    myAISContext->Display(myTrihedron);
    myAISContext->Deactivate(myTrihedron);
  }

  // selection
  mySelectionEnabled = true;
  myMultiSelectionEnabled = true;
}


OCCViewer_Viewer::~OCCViewer_Viewer() 
{
}

QColor OCCViewer_Viewer::backgroundColor() const
{
  return myBgColor;
}

void OCCViewer_Viewer::setBackgroundColor( const QColor& c )
{
  if ( c.isValid() )
    myBgColor = c;
}

SUIT_ViewWindow* OCCViewer_Viewer::createView( SUIT_Desktop* theDesktop )
{
  OCCViewer_ViewWindow* res = new OCCViewer_ViewWindow(theDesktop, this);
  res->initLayout();

  OCCViewer_ViewPort3d* vp3d = res->getViewPort();
  if ( vp3d )
    vp3d->setBackgroundColor( myBgColor );

  return res;
}

//*********************************************************************
void OCCViewer_Viewer::setViewManager(SUIT_ViewManager* theViewManager)
{
  SUIT_ViewModel::setViewManager(theViewManager);
  if (theViewManager) {
    connect(theViewManager, SIGNAL(mousePress(SUIT_ViewWindow*, QMouseEvent*)), 
            this, SLOT(onMousePress(SUIT_ViewWindow*, QMouseEvent*)));

    connect(theViewManager, SIGNAL(mouseMove(SUIT_ViewWindow*, QMouseEvent*)), 
            this, SLOT(onMouseMove(SUIT_ViewWindow*, QMouseEvent*)));

    connect(theViewManager, SIGNAL(mouseRelease(SUIT_ViewWindow*, QMouseEvent*)), 
            this, SLOT(onMouseRelease(SUIT_ViewWindow*, QMouseEvent*)));
  }
}


//*********************************************************************
void OCCViewer_Viewer::onMousePress(SUIT_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  myStartPnt.setX(theEvent->x()); myStartPnt.setY(theEvent->y());
}


//*********************************************************************
void OCCViewer_Viewer::onMouseMove(SUIT_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  if (!mySelectionEnabled) return;
  if (!theWindow->inherits("OCCViewer_ViewWindow")) return;

  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*) theWindow;
  myAISContext->MoveTo(theEvent->x(), theEvent->y(), aView->getViewPort()->getView());
}


//*********************************************************************
void OCCViewer_Viewer::onMouseRelease(SUIT_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  if (!mySelectionEnabled) return;
  if (theEvent->button() != Qt::LeftButton) return;
  if (!theWindow->inherits("OCCViewer_ViewWindow")) return;


  myEndPnt.setX(theEvent->x()); myEndPnt.setY(theEvent->y());
  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*) theWindow;
  bool aHasShift = (theEvent->state() & Qt::ShiftButton);

  if (myStartPnt == myEndPnt)
  {
    if (aHasShift && myMultiSelectionEnabled)
      myAISContext->ShiftSelect();
    else
      myAISContext->Select();
  }
  else
  {
    if (aHasShift && myMultiSelectionEnabled)
      myAISContext->ShiftSelect(myStartPnt.x(), myStartPnt.y(),
                                myEndPnt.x(), myEndPnt.y(),
                                aView->getViewPort()->getView(), Standard_False );
    else
      myAISContext->Select(myStartPnt.x(), myStartPnt.y(),
                           myEndPnt.x(), myEndPnt.y(),
                           aView->getViewPort()->getView(), Standard_False );

    int Nb = myAISContext->NbSelected();
    if( Nb>1 && !myMultiSelectionEnabled )
    {
        myAISContext->InitSelected();
        Handle( SelectMgr_EntityOwner ) anOwner = myAISContext->SelectedOwner();
        if( !anOwner.IsNull() )
        {
            myAISContext->ClearSelected( Standard_False );
            myAISContext->AddOrRemoveSelected( anOwner, Standard_False );
        }
    }

    myAISContext->UpdateCurrentViewer();
  }
  emit selectionChanged();
}


//*********************************************************************
void OCCViewer_Viewer::enableSelection(bool isEnabled)
{
  mySelectionEnabled = isEnabled;
  //!! To be done for view windows
}

//*********************************************************************
void OCCViewer_Viewer::enableMultiselection(bool isEnable)
{
  myMultiSelectionEnabled = isEnable;
  //!! To be done for view windows
}

//*********************************************************************
void OCCViewer_Viewer::contextMenuPopup(QPopupMenu* thePopup)
{
  thePopup->insertItem( tr( "MEN_DUMP_VIEW" ), this, SLOT( onDumpView() ) );
  thePopup->insertItem( tr( "MEN_CHANGE_BACKGROUD" ), this, SLOT( onChangeBgColor() ) );

  thePopup->insertSeparator();

  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*)(myViewManager->getActiveView());
  if ( aView && !aView->getToolBar()->isVisible() )
    thePopup->insertItem( tr( "MEN_SHOW_TOOLBAR" ), this, SLOT( onShowToolbar() ) );
}

void OCCViewer_Viewer::onDumpView()
{
  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*)(myViewManager->getActiveView());
  if ( aView )
    aView->onDumpView();
}

//*********************************************************************
void OCCViewer_Viewer::onChangeBgColor()
{
  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*)(myViewManager->getActiveView());
  if( !aView )
    return;
  OCCViewer_ViewPort3d* aViewPort3d = aView->getViewPort();
  if( !aViewPort3d )
    return;
  QColor aColorActive = aViewPort3d->backgroundColor();

  QColor selColor = QColorDialog::getColor( aColorActive, aView);
  if ( selColor.isValid() )
    aViewPort3d->setBackgroundColor(selColor);
}

//*********************************************************************
void OCCViewer_Viewer::onShowToolbar() {
  OCCViewer_ViewWindow* aView = (OCCViewer_ViewWindow*)(myViewManager->getActiveView());
  if ( aView )
    aView->getToolBar()->show();    
}

//*********************************************************************
void OCCViewer_Viewer::update()
{
  if (!myV3dViewer.IsNull())
    myV3dViewer->Update();
}

//*********************************************************************
void OCCViewer_Viewer::getSelectedObjects(AIS_ListOfInteractive& theList)
{
  theList.Clear();
  for (myAISContext->InitSelected(); myAISContext->MoreSelected(); myAISContext->NextSelected())
    theList.Append(myAISContext->SelectedInteractive());
}

//*********************************************************************
void OCCViewer_Viewer::setObjectsSelected(const AIS_ListOfInteractive& theList)
{
  AIS_ListIteratorOfListOfInteractive aIt;
  for (aIt.Initialize(theList); aIt.More(); aIt.Next())
    myAISContext->SetSelected(aIt.Value(), false);
  myAISContext->UpdateCurrentViewer();
}

//*********************************************************************
void OCCViewer_Viewer::performSelectionChanged()
{
    emit selectionChanged();
}

//****************************************************************

void OCCViewer_Viewer::onClearViewAspects()
{
    clearViewAspects();
}

//****************************************************************

void OCCViewer_Viewer::clearViewAspects()
{
	myViewAspects.clear();
}

//****************************************************************

const viewAspectList& OCCViewer_Viewer::getViewAspects()
{
	return myViewAspects;
}

//****************************************************************

void OCCViewer_Viewer::appendViewAspect( const viewAspect& aParams )
{
	myViewAspects.append( aParams );
}

//****************************************************************

void OCCViewer_Viewer::updateViewAspects( const viewAspectList& aViewList )
{
	myViewAspects = aViewList;
}

bool OCCViewer_Viewer::highlight( const Handle(AIS_InteractiveObject)& obj,
                                  bool hilight, bool update )
{
  bool isInLocal = myAISContext->HasOpenedContext();
  if( !obj.IsNull() )
    if( !isInLocal )
    {
      if ( hilight && !myAISContext->IsSelected( obj ) )
        myAISContext->AddOrRemoveCurrentObject( obj, false );
      else if ( !hilight && myAISContext->IsSelected( obj ) )
        myAISContext->AddOrRemoveCurrentObject( obj, false );
    }

  if ( update )
    myV3dViewer->Redraw();
    
  return false;
}

bool OCCViewer_Viewer::unHighlightAll( bool updateviewer )
{
  if ( myAISContext->HasOpenedContext() )
    myAISContext->ClearSelected( updateviewer );
  else
    myAISContext->ClearCurrents( updateviewer );
  return false;
}

bool OCCViewer_Viewer::isInViewer( const Handle(AIS_InteractiveObject)& obj,
                                   bool onlyInViewer )
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);

  if( !onlyInViewer )
  {
    AIS_ListOfInteractive List1;
    myAISContext->ObjectsInCollector(List1);
    List.Append(List1);
  }

  AIS_ListIteratorOfListOfInteractive ite(List);
  for ( ; ite.More(); ite.Next() )
    if( ite.Value()==obj )
      return true;

  return false;
}

bool OCCViewer_Viewer::isVisible( const Handle(AIS_InteractiveObject)& obj )
{
  return myAISContext->IsDisplayed( obj );
}

void OCCViewer_Viewer::setColor( const Handle(AIS_InteractiveObject)& obj,
                                 const QColor& color,
                                 bool update )
{
  if( !obj.IsNull() )
  {
    Quantity_Color CSFColor = Quantity_Color ( color.red() / 255.,
                                               color.green() / 255.,
                                               color.blue() / 255.,
                                               Quantity_TOC_RGB );
    obj->SetColor( CSFColor );
  }

  if( update )
    myV3dViewer->Update();
}

void OCCViewer_Viewer::switchRepresentation( const Handle(AIS_InteractiveObject)& obj,
                                             int mode, bool update )
{
  myAISContext->SetDisplayMode( obj, (Standard_Integer)mode, true );
  if( update )
    myV3dViewer->Update();
}

void OCCViewer_Viewer::setTransparency( const Handle(AIS_InteractiveObject)& obj,
                                        float trans, bool update )
{
  myAISContext->SetTransparency( obj, trans, false );
  myAISContext->Redisplay( obj, Standard_False, Standard_True );
  if( update )
    myV3dViewer->Update();
}

//****************************************************************
void OCCViewer_Viewer::toggleTrihedron()
{
  setTrihedronShown( !isTrihedronVisible() );
}

bool OCCViewer_Viewer::isTrihedronVisible() const
{
  return !myTrihedron.IsNull() && !myAISContext.IsNull() && myAISContext->IsDisplayed( myTrihedron );
}

void OCCViewer_Viewer::setTrihedronShown( const bool on )
{
  if ( myTrihedron.IsNull() )
    return;

  if ( on )
    myAISContext->Display( myTrihedron );
  else
    myAISContext->Erase( myTrihedron );
}

int OCCViewer_Viewer::trihedronSize() const
{
  int sz = 0;
  if ( !myTrihedron.IsNull() )
    sz = (int)myTrihedron->Size();
  return sz;
}

void OCCViewer_Viewer::setTrihedronSize( const int sz )
{
  if ( !myTrihedron.IsNull() )
    myTrihedron->SetSize( sz );
}

void OCCViewer_Viewer::setIsos( const int u, const int v )
{
  Handle(AIS_InteractiveContext) ic = getAISContext();
  if ( ic.IsNull() )
  return;

  ic->SetIsoNumber( u, AIS_TOI_IsoU );
  ic->SetIsoNumber( u, AIS_TOI_IsoV );
}

void OCCViewer_Viewer::isos( int& u, int& v ) const
{
  Handle(AIS_InteractiveContext) ic = getAISContext();
  if ( !ic.IsNull() )
  {
    u = ic->IsoNumber( AIS_TOI_IsoU );
    v = ic->IsoNumber( AIS_TOI_IsoV );
  }
}
