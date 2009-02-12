dnl  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl  This library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public
dnl  License as published by the Free Software Foundation; either
dnl  version 2.1 of the License.
dnl
dnl  This library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with this library; if not, write to the Free Software
dnl  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
dnl
dnl  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl
#  Check availability of Salome binary distribution
#
#  Author : Marc Tajchman (CEA, 2002)
#------------------------------------------------------------

AC_DEFUN([CHECK_GUI],[
AC_REQUIRE([AC_LINKER_OPTIONS])dnl

AC_CHECKING(for $2 $1 )

SalomeGUI_ok=no

GUI_LDFLAGS=""
GUI_CXXFLAGS=""

AC_ARG_WITH(gui,
	    --with-salome_gui=DIR root directory path of SALOME GUI installation,
	    SALOME_GUI_DIR="$withval",SALOME_GUI_DIR="")

if test "x${SALOME_GUI_DIR}" = "x" ; then
  # no --with-gui-dir option used
  AC_MSG_RESULT(try \${GUI_ROOT_DIR}: ${GUI_ROOT_DIR})
  if test "x${GUI_ROOT_DIR}" != "x" ; then
    # SALOME_ROOT_DIR environment variable defined
    SALOME_GUI_DIR=${GUI_ROOT_DIR}
  else
    # search Salome binaries in PATH variable
    AC_PATH_PROG(TEMP, $1)
    if test "x${TEMP}" != "x" ; then
      AC_MSG_RESULT($1 was found at : ${TEMP})
      SALOME_BIN_DIR=`dirname ${TEMP}`
      SALOME_GUI_DIR=`cd ${SALOME_BIN_DIR}/../..; pwd`
    fi
  fi
fi

if test -f ${SALOME_GUI_DIR}/bin/salome/$1 ; then
  SalomeGUI_ok=yes
  AC_MSG_RESULT(Using SALOME GUI distribution in ${SALOME_GUI_DIR})

  if test "x${GUI_ROOT_DIR}" == "x" ; then
    GUI_ROOT_DIR=${SALOME_GUI_DIR}
  fi

  AC_SUBST(GUI_ROOT_DIR)

  GUI_LDFLAGS=-L${SALOME_GUI_DIR}/lib${LIB_LOCATION_SUFFIX}/salome
  GUI_CXXFLAGS=-I${SALOME_GUI_DIR}/include/salome

  AC_SUBST(GUI_LDFLAGS)
  AC_SUBST(GUI_CXXFLAGS)
else
  AC_MSG_WARN("Cannot find compiled SALOME GUI distribution")
fi
  
AC_MSG_RESULT(for $2: ${SalomeGUI_ok})
 
])dnl

AC_DEFUN([CHECK_SALOME_GUI],[
  CHECK_GUI([SUITApp],
            [SALOME GUI])
])dnl
