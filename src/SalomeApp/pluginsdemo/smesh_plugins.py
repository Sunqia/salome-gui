# -*- coding: utf-8 -*-
#!/usr/bin/env python
# Copyright (C) 2010-2015  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# Author : Guillaume Boulant (EDF)

import salome_pluginsmanager

DEMO_IS_ACTIVATED = True

from minmax_plugin import *
 
# register the function in the plugin manager
if DEMO_IS_ACTIVATED:
  salome_pluginsmanager.AddFunction('Get min or max value of control',
      'Get min or max value of control',
      minmax)
