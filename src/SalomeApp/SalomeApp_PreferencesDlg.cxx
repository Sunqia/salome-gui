// File:      SalomeApp_PreferencesDlg.cxx
// Author:    Sergey TELKOV

#include "SalomeApp_PreferencesDlg.h"

#include "SalomeApp_Preferences.h"

#include <qvbox.h>
#include <qlayout.h>

SalomeApp_PreferencesDlg::SalomeApp_PreferencesDlg( SalomeApp_Preferences* prefs, QWidget* parent )
: QtxDialog( parent, 0, true, false, Standard | Apply ),
myPrefs( prefs )
{
  setCaption( tr( "CAPTION" ) );

  QVBoxLayout* main = new QVBoxLayout( mainFrame(), 5 );

  QVBox* base = new QVBox( mainFrame() );
  main->addWidget( base );

  myPrefs->reparent( base, QPoint( 0, 0 ), true );

  setFocusProxy( myPrefs );

  setDialogFlags( AlignOnce );

  connect( this, SIGNAL( dlgHelp() ),  this, SLOT( onHelp() ) );
  connect( this, SIGNAL( dlgApply() ), this, SLOT( onApply() ) );
}

SalomeApp_PreferencesDlg::~SalomeApp_PreferencesDlg()
{
  if ( !myPrefs )
    return;

  myPrefs->reparent( 0, QPoint( 0, 0 ), false );
  myPrefs = 0;
}

void SalomeApp_PreferencesDlg::show()
{
  myPrefs->retrieve();
  myPrefs->toBackup();

  QtxDialog::show();
}

void SalomeApp_PreferencesDlg::accept()
{
  QtxDialog::accept();

  myPrefs->store();
}

void SalomeApp_PreferencesDlg::reject()
{
  QtxDialog::reject();

  myPrefs->fromBackup();
}

void SalomeApp_PreferencesDlg::onHelp()
{
}

void SalomeApp_PreferencesDlg::onApply()
{
  myPrefs->store();
}
