# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2010 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

IDL=		$(OMNIORB_BIN)\omniidl.exe
IDL_OPTS=	-bcxx -Wbh=.h,s=.cpp -C$(IDL_OUT_DIR)
IDL_INCLUDES=	-I../../modules/tweek
IDL_OUT_DIR=	..\Tweek_CXX\tweek\idl 
IDL_SRC_DIR=	..\..\modules\tweek\tweek\idl

$(IDL_OUT_DIR):
	mkdir $(IDL_OUT_DIR)

$(IDL_OUT_DIR)\Observer.cpp $(IDL_OUT_DIR)\Observer.h:	\
   $(IDL_SRC_DIR)\Observer.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_SRC_DIR)\Observer.idl

$(IDL_OUT_DIR)\Subject.cpp $(IDL_OUT_DIR)\Subject.h:	\
   $(IDL_SRC_DIR)\Subject.idl $(IDL_SRC_DIR)\Observer.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_SRC_DIR)\Subject.idl

$(IDL_OUT_DIR)\SubjectManager.cpp $(IDL_OUT_DIR)\SubjectManager.h:	\
   $(IDL_SRC_DIR)\SubjectManager.idl $(IDL_SRC_DIR)\Subject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_SRC_DIR)\SubjectManager.idl

$(IDL_OUT_DIR)\BeanDeliverySubject.cpp $(IDL_OUT_DIR)\BeanDeliverySubject.h: \
   $(IDL_SRC_DIR)\BeanDeliverySubject.idl $(IDL_SRC_DIR)\Subject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES)		\
           $(IDL_SRC_DIR)\BeanDeliverySubject.idl

BASE_OUTPUT=	\
		$(IDL_OUT_DIR)\Observer.cpp		\
		$(IDL_OUT_DIR)\Observer.h		\
		$(IDL_OUT_DIR)\Subject.cpp		\
		$(IDL_OUT_DIR)\Subject.h		\
		$(IDL_OUT_DIR)\SubjectManager.cpp	\
		$(IDL_OUT_DIR)\SubjectManager.h

SUBJECT_OUTPUT=	\
		$(IDL_OUT_DIR)\BeanDeliverySubject.cpp		\
		$(IDL_OUT_DIR)\BeanDeliverySubject.h

OUTPUT=		$(BASE_OUTPUT)		\
		$(SUBJECT_OUTPUT)

all: $(IDL_OUT_DIR) $(OUTPUT)

clean:
	-rmdir /S /Q $(IDL_OUT_DIR)
