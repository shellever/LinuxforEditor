#ifndef UDEF_H
#define UDEF_H

#define _DEBUG              //switch to debug the program
//#define __UNIX            //unix platform supported

#ifdef _DEBUG

#include <QDebug>
#define _off                    0
#define _on                     1
#define _debug_dropEvent_       _off     // 03/01/2015 ok
#define _debug_setCurrentFile_  _off
#define _debug_process_         _on
#define _debug_getKeyword_      _off     // 03/03/2015 ok
#define _debug_preprocessor_    _off      // 03/04/2015 ok
#endif

#endif // UDEF_H
