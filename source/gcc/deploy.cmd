set projname=dbridge_bt_eventTracker

set sharedir=%OnControlDev%
if not EXIST %sharedir% mkdir %sharedir%

set sharedirproj=%sharedir%\Databridge\plugins\%projname%
if not EXIST %sharedirproj% mkdir %sharedirproj%


if not EXIST %sharedirproj%\img mkdir %sharedirproj%\img
xcopy /sY ..\img\* %sharedirproj%\img

xcopy /sY Release\eventTracker.dbp %sharedirproj%
