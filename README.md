# AppToWinService
An example to run a windows service from executable

Add *"DAEMON"* macros to run a app to windows service.  

Then create service by typing command:
```
sc.exe create TEST binPath=<your .exe path>
```

