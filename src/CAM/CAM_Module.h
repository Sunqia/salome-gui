#ifndef CAM_MODULE_H
#define CAM_MODULE_H

#include "CAM.h"

#include <qpixmap.h>
#include <qobject.h>
#include <qpopupmenu.h>

class QAction;
class SUIT_Study;
class CAM_DataModel;
class CAM_Application;
class QtxActionMenuMgr;
class QtxActionToolMgr;

#ifdef WIN32
#pragma warning( disable: 4251 )
#endif

class CAM_EXPORT CAM_Module : public QObject
{
  Q_OBJECT

public:
  CAM_Module();
  CAM_Module( const QString& );
  virtual ~CAM_Module();

  virtual void           initialize( CAM_Application* );

  QString                moduleName() const;
  QPixmap                moduleIcon() const;

  CAM_DataModel*         dataModel() const;
  CAM_Application*       application() const;

  virtual void           contextMenuPopup( const QString&, QPopupMenu*, QString& title ) {};
  virtual void           updateCommandsStatus() {};

public slots:
  virtual bool           activateModule( SUIT_Study* );
  virtual bool           deactivateModule( SUIT_Study* );

  virtual void           studyClosed( SUIT_Study* );
  virtual void           studyChanged( SUIT_Study*, SUIT_Study* );

protected: 
  virtual CAM_DataModel* createDataModel();

  virtual void           setModuleName( const QString& );
  virtual void           setModuleIcon( const QPixmap& );

  QtxActionMenuMgr*      menuMgr() const;
  QtxActionToolMgr*      toolMgr() const;

  int                    createTool( const QString& );
  int                    createTool( const int, const int, const int = -1 );
  int                    createTool( const int, const QString&, const int = -1 );
  int                    createTool( QAction*, const int, const int = -1, const int = -1 );
  int                    createTool( QAction*, const QString&, const int = -1, const int = -1 );

  int                    createMenu( const QString&, const int, const int = -1, const int = -1, const int = -1 );
  int                    createMenu( const QString&, const QString&, const int = -1, const int = -1, const int = -1 );
  int                    createMenu( const int, const int, const int = -1, const int = -1 );
  int                    createMenu( const int, const QString&, const int = -1, const int = -1 );
  int                    createMenu( QAction*, const int, const int = -1, const int = -1, const int = -1 );
  int                    createMenu( QAction*, const QString&, const int = -1, const int = -1, const int = -1 );

  void                   setMenuShown( const bool );
  void                   setMenuShown( QAction*, const bool );
  void                   setMenuShown( const int, const bool );

  void                   setToolShown( const bool );
  void                   setToolShown( QAction*, const bool );
  void                   setToolShown( const int, const bool );

  static QAction*        separator();

  QAction*               action( const int ) const;
  int                    actionId( const QAction* ) const;

  int                    registerAction( const int, QAction* );
  QAction*               createAction( const int, const QString&, const QIconSet&, const QString&,
                                       const QString&, const int, QObject* = 0,
                                       const bool = false, QObject* = 0, const char* = 0 );

private:
  CAM_Application*       myApp;
  QString                myName;
  QPixmap                myIcon;
  CAM_DataModel*         myDataModel;
  QMap<int, QAction*>    myActionMap;

  friend class CAM_Application;
};

#ifdef WIN32
#pragma warning( default: 4251 )
#endif

extern "C" {
  typedef CAM_Module* (*GET_MODULE_FUNC)();
}

#define GET_MODULE_NAME "createModule"

#endif
