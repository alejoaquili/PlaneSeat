#!/bin/bash
xterm -title "App 1" -hold -e ./Server/server.out   &
xterm -title "App 2" -hold -e ./Client/client.out   &
xterm -title "App 3" -hold -e ./Client/client.out   &
xterm -title "App 4" -hold -e ./Client/client.out   &