_KNIGHT = "D:\\Code\\GitHub\\VEDO.dev\\bin\\Code_Blocks\\Knight.exe"

import os
import shutil

def _create_an_empty_folder(folder):
	if os.path.exists(folder):
		shutil.rmtree(folder)
	os.mkdir(folder)

_create_an_empty_folder("vpf")
_create_an_empty_folder("vtu")

_TARGETS = os.listdir("./")

for _TARGET in _TARGETS:
	if _TARGET[-4:] == ".ido":
		_MESSAGE = "Dealing with the file \"" + _TARGET + "\"..."
		print(_MESSAGE)
		_TARGET_VPF = _TARGET[:-4] + ".vpf"
		_COMMAND = _KNIGHT + " -convert " + _TARGET + " vpf/" + _TARGET_VPF
		os.system(_COMMAND)
		_TARGET_VTU = _TARGET[:-4] + ".vtu"
		_COMMAND = _KNIGHT + " -convert " + _TARGET + " vtu/" + _TARGET_VTU
		os.system(_COMMAND)
		_TARGET_IDO = "ido/" + _TARGET
