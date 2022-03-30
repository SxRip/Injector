![Logo](https://github.com/SxRip/Injector/blob/main/assets/Injector.png)

⚠ The name of the injector must be in this form: **ProcessToInject**.**dllName**.exe

For compilation: 
* Go to Project Properties ->
  * Configuration properties -> Additionally
  * Character set -> Use multibyte encoding

⚠ After the compilation you can see that injector does not works.
It happened because you running it without administrator permissions.

You can fix that:
* Go to Project Properties ->
  * Linker -> File manifest
  * UAC runlevel -> requireAdministrator (/level='requireAdministrator')

