//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_base.h
//  Author : Christian CAREMOLI, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _PYINTERP_BASE_H_
#define _PYINTERP_BASE_H_

#include "PyInterp.h"

#include <list>
#include <string>
#include <iostream>

// include order important!
// pthread then python then qt
//#include <pthread.h>  // must be before Python.h !

#include <Python.h>   // must be before qt includes ...

#ifndef WNT
extern "C" PyObject * PyEval_EvalCode(PyObject *co, PyObject *g, PyObject *l);
#endif

#define TOP_HISTORY_PY "--- top of history ---"
#define BEGIN_HISTORY_PY "--- begin of history ---"

class PYINTERP_EXPORT PyLockWrapper
{
  PyThreadState* myThreadState;
  PyThreadState* mySaveThreadState;
 public:
  PyLockWrapper(PyThreadState* theThreadState);
  ~PyLockWrapper();
};


class PYINTERP_EXPORT PyInterp_base{
 public:
  static int _argc;
  static char* _argv[];
  static PyObject *builtinmodule;
  
  PyInterp_base();
  ~PyInterp_base();
  
  virtual void initialize();
  static void init_python();

  virtual int run(const char *command); 

  PyLockWrapper GetLockWrapper();

  std::string getbanner(); 
  std::string getverr();
  std::string getvout();  

  const char * getPrevious();
  const char * getNext();    

 protected:
  PyThreadState * _tstate;
  PyObject * _vout;
  PyObject * _verr;
  PyObject * _g;
  PyObject * _codeop;
  std::list<std::string> _history;
  std::list<std::string>::iterator _ith;
  bool _atFirst;

  int simpleRun(const char* command);
  int initRun();

  virtual bool initState() = 0;
  virtual bool initContext() = 0;  
};


class PYINTERP_EXPORT PyObjWrapper{
  PyObject* myObject;
public:
  PyObjWrapper(PyObject* theObject): myObject(theObject) {}
  PyObjWrapper(): myObject(0) {}
  operator PyObject*(){
    return myObject;
  }
  PyObject* operator->(){
    return myObject;
  }
  PyObject* get(){
    return myObject;
  }
  bool operator!(){
    return !myObject;
  }
  bool operator==(PyObject* theObject){
    return myObject == theObject;
  }
  PyObject** operator&(){
    return &myObject;
  }
  PyObjWrapper& operator=(PyObjWrapper* theObjWrapper){
    Py_XDECREF(myObject);
    myObject = theObjWrapper->myObject;
    return *this;
  }
  virtual ~PyObjWrapper(){ 
    Py_XDECREF(myObject);
  }
};

#endif
