_KNIGHT = "D:\\Code\\GitHub\\VEDO.dev\\bin\\Code_Blocks\\Knight.exe"

import os
import shutil

def _create_an_empty_folder(folder):
	if os.path.exists(folder):
		shutil.rmtree(folder)
	os.mkdir(folder)

def _create_a_folder(folder):
	if os.path.exists(folder):
		_MESSAGE = "Skip create folder \"" + folder + "\""
		print(_MESSAGE)
	else:
		os.mkdir(folder)

_create_an_empty_folder("vpf")
_create_an_empty_folder("vtu")
_create_an_empty_folder("basic")
_create_a_folder("basic/log")
_create_a_folder("ido")

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
		shutil.move(_TARGET, _TARGET_IDO)
	elif _TARGET[-4:] == ".txt" or _TARGET[-4:] == ".csv":
		_TARGET_TEXT = "basic/log/" + _TARGET
		shutil.move(_TARGET, _TARGET_TEXT)
	elif _TARGET[-4:] == ".xml" or _TARGET[-3:] == ".sh" or _TARGET[-3:] == ".py":
		_TARGET_TEXT = "basic/" + _TARGET
		shutil.move(_TARGET, _TARGET_TEXT)
	elif os.path.isdir(_TARGET):
		print()
	else:
		os.remove(_TARGET)
