#
# This file is part of the coreboot project.
#
# Copyright (C) 2012 Advanced Micro Devices, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#

romstage-y += update_microcode.c

ramstage-y += chip_name.c
ramstage-y += model_16_init.c
ramstage-y += update_microcode.c


subdirs-y += ../../mtrr
subdirs-y += ../../../x86/tsc
subdirs-y += ../../../x86/lapic
subdirs-y += ../../../x86/cache
subdirs-y += ../../../x86/mtrr
subdirs-y += ../../../x86/pae
subdirs-y += ../../../x86/smm
