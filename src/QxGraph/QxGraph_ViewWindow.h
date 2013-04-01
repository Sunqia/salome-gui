// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME QxGraph : build Supervisor viewer into desktop
//
#ifndef QXGRAPH_VIEWWINDOW_H
#define QXGRAPH_VIEWWINDOW_H

#include "QxGraph.h"
#include "QxGraph_ViewModel.h"

#include "SUIT_ViewWindow.h"

#include "QtxAction.h"

#include <qaction.h>

#ifdef WIN32
#pragma warning ( disable:4251 )
#endif

class SUIT_ResourceMgr;

class QXGRAPH_EXPORT QxGraph_ViewWindow : public SUIT_ViewWindow {
  Q_OBJECT

 public:
  QxGraph_ViewWindow(SUIT_Desktop* theDesktop, QxGraph_Viewer* theModel);
  virtual ~QxGraph_ViewWindow();

  void              setBackgroundColor( const QColor& );
  QColor            backgroundColor() const;

  QToolBar*         getToolBar() { return myToolBar; }

  SUIT_ResourceMgr* resMgr() const;

  QxGraph_Viewer*   getViewModel() const { return myViewModel; }
  void              setViewModel(QxGraph_Viewer* theViewModel) { myViewModel = theViewModel; }

  virtual void      initLayout();

  void              contextPopupEvent(QContextMenuEvent* theEvent) { emit contextMenuRequested( theEvent); }
  
 public slots:
  void              onViewFitAll();
  void              onViewFitArea();
  void              onViewZoom();
  void              onViewPan(); 
  void              onViewGlobalPan(); 
  void              onViewReset(); 
  
 protected:
  void              resizeEvent( QResizeEvent* theEvent );

  virtual void      initCanvas();
  virtual void      initCanvasViews();

 private:
  void              createActions();
  void              createToolBar();

  //! Actions ID
  enum { FitAllId, FitRectId, ZoomId, PanId, GlobalPanId, ResetId };
  typedef QMap<int, QtxAction*> ActionsMap;

  ActionsMap        myActionsMap;
  QToolBar*         myToolBar;

  QxGraph_Viewer*   myViewModel;
  
};

#ifdef WIN32
#pragma warning ( default:4251 )
#endif

#endif
