![Logo](https://github.com/SxRip/Injector/blob/main/assets/Injector.png)

⚠ The name of the injector must be in this form: **ProcessToInject**.**dllName**.exe

⚠ The DLL for inject should be in that folder, where injector exist.

For compilation: 
* Go to Project Properties ->
  * Configuration properties -> Additionally
  * Character set -> Use multibyte encoding

⚠ After compilation, the injector may not work. This happened because you were running it without admin rights.

You can fix that:
* Go to Project Properties ->
  * Linker -> File manifest
  * UAC runlevel -> requireAdministrator (/level='requireAdministrator')

