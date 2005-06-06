//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_Selection.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SVTK_SELECTORDEF_H
#define SVTK_SELECTORDEF_H

#include <set>
#include <map>

#include <TColStd_IndexedMapOfInteger.hxx>
#include <TColStd_MapOfInteger.hxx>
#include <vtkSmartPointer.h>

#include "SVTK_Selector.h"

class SALOME_Actor;

class SVTK_Viewer;
class SVTK_ViewWindow;

class SVTK_SelectorDef: public SVTK_Selector
{
public:
  SVTK_SelectorDef();
  virtual ~SVTK_SelectorDef();

  virtual
  void 
  SetSelectionMode( Selection_Mode theMode );

  virtual
  Selection_Mode
  SelectionMode() const { return mySelectionMode; }

  virtual
  void
  ClearIObjects();

  virtual
  SALOME_Actor* 
  GetActor(const Handle(SALOME_InteractiveObject)& theIO) const;

  virtual
  bool
  IsSelected(const Handle(SALOME_InteractiveObject)& theIO) const;

  virtual
  bool
  IsSelected(SALOME_Actor* theActor) const;

  virtual
  bool
  AddIObject(const Handle(SALOME_InteractiveObject)& theIO);

  virtual
  bool
  AddIObject(SALOME_Actor* theActor);

  virtual
  bool 
  RemoveIObject(const Handle(SALOME_InteractiveObject)& theIO);

  virtual
  bool 
  RemoveIObject(SALOME_Actor* theActor);

  virtual
  const SALOME_ListIO& 
  StoredIObjects() const;

  virtual
  int 
  IObjectCount() const;

  virtual
  bool 
  HasIndex(const Handle(SALOME_InteractiveObject)& theIO ) const;

  virtual
  void 
  GetIndex( const Handle(SALOME_InteractiveObject)& theIO, 
	    TColStd_IndexedMapOfInteger& theIndex );
	
  virtual
  bool 
  AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& theIO, 
		    const TColStd_IndexedMapOfInteger& theIndices, 
		    bool theIsModeShift);
  virtual
  bool 
  AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& theIO, 
		    const TColStd_MapOfInteger& theIndices, 
		    bool theIsModeShift);
  virtual
  bool
  AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& theIO, 
		    int theIndex, 
		    bool theIsModeShift);
  virtual
  void 
  RemoveIndex( const Handle(SALOME_InteractiveObject)& theIO, 
	       int theIndex);
  virtual
  bool 
  IsIndexSelected(const Handle(SALOME_InteractiveObject)& theIO, 
		  int theIndex) const;
  virtual
  void 
  ClearIndex();

private:
  Selection_Mode mySelectionMode;

  struct TIOLessThan
  {
    bool 
    operator()(const Handle(SALOME_InteractiveObject)& theRightIO,
	       const Handle(SALOME_InteractiveObject)& theLeftIO) const
    {
      return strcmp(theRightIO->getEntry(),theLeftIO->getEntry()) < 0;
    }
  };

  struct TIndexedMapOfInteger{
    TColStd_IndexedMapOfInteger myMap;
    TIndexedMapOfInteger()
    {}
    TIndexedMapOfInteger(const TIndexedMapOfInteger& theIndexedMapOfInteger)
    {
      myMap = theIndexedMapOfInteger.myMap;
    }
  };

  mutable SALOME_ListIO myIObjectList;
  typedef std::set<Handle(SALOME_InteractiveObject),
                   TIOLessThan> TIObjects;
  TIObjects myIObjects;

  typedef std::map<Handle(SALOME_InteractiveObject),
                   vtkSmartPointer<SALOME_Actor>,
                   TIOLessThan> TIO2Actors;
  TIO2Actors myIO2Actors;

  typedef std::map<Handle(SALOME_InteractiveObject),
                   TIndexedMapOfInteger,
                   TIOLessThan> TMapIOSubIndex;
  TMapIOSubIndex myMapIOSubIndex;
};


#endif