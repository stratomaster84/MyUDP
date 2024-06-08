5. ver 0.1.3
	- отсылка к двум сокетам (client/server) для возможности обеспечения многопоточности
	Если будет необходимость - СОВМЕСТИМ
4. ver 0.1.2beta
	- проверена возможность совмещения двух сокетов - ПОЛОЖИТЕЛЬНО
	однако будем работать по прошлой схеме (два сокета = client/server)
	т.к. возможно потребуется многопоточность
3. ver 0.1.2
	- some error texts are changed
	- в UdpTrans добавлена public::getPort()
	- добавление UNKNOWN адреса
2. ver 0.1.1 - Second version of "MyUDP"
	- assembled with CMake
1. ver 0.1.0 - First version of "MyUDP" (assembled with QMake)