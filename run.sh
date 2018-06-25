#!/bin/bash
#x-terminal-emulator -title "App 1" -hold -e ./Server/server.out   &
#x-terminal-emulator -title "App 2" -hold -e ./Client/client.out   &
#x-terminal-emulator -title "App 3" -hold -e ./Client/client.out   &
#x-terminal-emulator -title "App 4" -hold -e ./Client/client.out   &
osascript -e 'tell app "Terminal" to do script "/Server/server.out"'
osascript -e 'tell app "Terminal" to do script "./Client/client.out"'
osascript -e 'tell app "Terminal" to do script "./Client/client.out"'
osascript -e 'tell app "Terminal" to do script "./Client/client.out"'
