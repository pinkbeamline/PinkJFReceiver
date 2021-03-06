//
// File generated by rootcint at Thu Mar 29 16:09:00 2012

// Do NOT change. Changes will be lost next time file is generated
//

#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "TSlsDetectorDict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void energyCalibration_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void energyCalibration_Dictionary();
   static void *new_energyCalibration(void *p = 0);
   static void *newArray_energyCalibration(Long_t size, void *p);
   static void delete_energyCalibration(void *p);
   static void deleteArray_energyCalibration(void *p);
   static void destruct_energyCalibration(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::energyCalibration*)
   {
      ::energyCalibration *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::energyCalibration),0);
      static ::ROOT::TGenericClassInfo 
         instance("energyCalibration", "./energyCalibration.h", 168,
                  typeid(::energyCalibration), DefineBehavior(ptr, ptr),
                  0, &energyCalibration_Dictionary, isa_proxy, 0,
                  sizeof(::energyCalibration) );
      instance.SetNew(&new_energyCalibration);
      instance.SetNewArray(&newArray_energyCalibration);
      instance.SetDelete(&delete_energyCalibration);
      instance.SetDeleteArray(&deleteArray_energyCalibration);
      instance.SetDestructor(&destruct_energyCalibration);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::energyCalibration*)
   {
      return GenerateInitInstanceLocal((::energyCalibration*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::energyCalibration*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void energyCalibration_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const ::energyCalibration*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   void angularCalibration_ShowMembers(void *obj, TMemberInspector &R__insp, char *R__parent);
   static void angularCalibration_Dictionary();
   static void *new_angularCalibration(void *p = 0);
   static void *newArray_angularCalibration(Long_t size, void *p);
   static void delete_angularCalibration(void *p);
   static void deleteArray_angularCalibration(void *p);
   static void destruct_angularCalibration(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::angularCalibration*)
   {
      ::angularCalibration *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::angularCalibration),0);
      static ::ROOT::TGenericClassInfo 
         instance("angularCalibration", "./angularCalibration.h", 35,
                  typeid(::angularCalibration), DefineBehavior(ptr, ptr),
                  0, &angularCalibration_Dictionary, isa_proxy, 0,
                  sizeof(::angularCalibration) );
      instance.SetNew(&new_angularCalibration);
      instance.SetNewArray(&newArray_angularCalibration);
      instance.SetDelete(&delete_angularCalibration);
      instance.SetDeleteArray(&deleteArray_angularCalibration);
      instance.SetDestructor(&destruct_angularCalibration);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::angularCalibration*)
   {
      return GenerateInitInstanceLocal((::angularCalibration*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::angularCalibration*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void angularCalibration_Dictionary() {
      ::ROOT::GenerateInitInstanceLocal((const ::angularCalibration*)0x0)->GetClass();
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_energyCalibration(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::energyCalibration : new ::energyCalibration;
   }
   static void *newArray_energyCalibration(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::energyCalibration[nElements] : new ::energyCalibration[nElements];
   }
   // Wrapper around operator delete
   static void delete_energyCalibration(void *p) {
      delete ((::energyCalibration*)p);
   }
   static void deleteArray_energyCalibration(void *p) {
      delete [] ((::energyCalibration*)p);
   }
   static void destruct_energyCalibration(void *p) {
      typedef ::energyCalibration current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::energyCalibration

namespace ROOT {
   // Wrappers around operator new
   static void *new_angularCalibration(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::angularCalibration : new ::angularCalibration;
   }
   static void *newArray_angularCalibration(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::angularCalibration[nElements] : new ::angularCalibration[nElements];
   }
   // Wrapper around operator delete
   static void delete_angularCalibration(void *p) {
      delete ((::angularCalibration*)p);
   }
   static void deleteArray_angularCalibration(void *p) {
      delete [] ((::angularCalibration*)p);
   }
   static void destruct_angularCalibration(void *p) {
      typedef ::angularCalibration current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::angularCalibration

/********************************************************
* TSlsDetectorDict.cpp
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && (__GNUC__ > 3) && (__GNUC_MINOR__ > 1)
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableTSlsDetectorDict();

extern "C" void G__set_cpp_environmentTSlsDetectorDict() {
  G__add_compiledheader("TObject.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("energyCalibration.h");
  G__add_compiledheader("angularCalibration.h");
  G__cpp_reset_tagtableTSlsDetectorDict();
}
#include <new>
extern "C" int G__cpp_dllrevTSlsDetectorDict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* energyCalibration */
static int G__TSlsDetectorDict_85_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   energyCalibration* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new energyCalibration[n];
     } else {
       p = new((void*) gvp) energyCalibration[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new energyCalibration;
     } else {
       p = new((void*) gvp) energyCalibration;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_85_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   switch (libp->paran) {
   case 1:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setPlotFlag((int) G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setPlotFlag());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_85_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   switch (libp->paran) {
   case 1:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setScanSign((int) G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setScanSign());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_85_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   switch (libp->paran) {
   case 1:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setChargeSharing((int) G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7, 105, (long) ((energyCalibration*) G__getstructoffset())->setChargeSharing());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__TSlsDetectorDict_85_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   energyCalibration* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new energyCalibration(*(energyCalibration*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef energyCalibration G__TenergyCalibration;
static int G__TSlsDetectorDict_85_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 0
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (energyCalibration*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((energyCalibration*) (soff+(sizeof(energyCalibration)*i)))->~G__TenergyCalibration();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (energyCalibration*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((energyCalibration*) (soff))->~G__TenergyCalibration();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__TSlsDetectorDict_85_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   energyCalibration* dest = (energyCalibration*) G__getstructoffset();
   *dest = *(energyCalibration*) libp->para[0].ref;
   const energyCalibration& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* angularCalibration */
static int G__TSlsDetectorDict_87_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   angularCalibration* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new angularCalibration[n];
     } else {
       p = new((void*) gvp) angularCalibration[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new angularCalibration;
     } else {
       p = new((void*) gvp) angularCalibration;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   switch (libp->paran) {
   case 1:
      G__letint(result7, 105, (long) ((angularCalibration*) G__getstructoffset())->setDirection((int) G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7, 105, (long) ((angularCalibration*) G__getstructoffset())->setDirection());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 102, (double) ((angularCalibration*) G__getstructoffset())->setEncoder((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 102, (double) ((angularCalibration*) G__getstructoffset())->getEncoder());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 102, (double) ((angularCalibration*) G__getstructoffset())->setTotalOffset((double) G__double(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 102, (double) ((angularCalibration*) G__getstructoffset())->getTotalOffset());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((angularCalibration*) G__getstructoffset())->setAngularRange((double) G__double(libp->para[0]), (double) G__double(libp->para[1]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__TSlsDetectorDict_87_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((angularCalibration*) G__getstructoffset())->getAngularRange(*(double*) G__doubleref(&libp->para[0]), *(double*) G__doubleref(&libp->para[1]));
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__TSlsDetectorDict_87_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   angularCalibration* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new angularCalibration(*(angularCalibration*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef angularCalibration G__TangularCalibration;
static int G__TSlsDetectorDict_87_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 0
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (angularCalibration*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((angularCalibration*) (soff+(sizeof(angularCalibration)*i)))->~G__TangularCalibration();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (angularCalibration*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((angularCalibration*) (soff))->~G__TangularCalibration();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__TSlsDetectorDict_87_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   angularCalibration* dest = (angularCalibration*) G__getstructoffset();
   *dest = *(angularCalibration*) libp->para[0].ref;
   const angularCalibration& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* energyCalibration */

/* angularCalibration */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncTSlsDetectorDict {
 public:
  G__Sizep2memfuncTSlsDetectorDict(): p(&G__Sizep2memfuncTSlsDetectorDict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncTSlsDetectorDict::*p)();
};

size_t G__get_sizep2memfuncTSlsDetectorDict()
{
  G__Sizep2memfuncTSlsDetectorDict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceTSlsDetectorDict() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableTSlsDetectorDict() {

   /* Setting up typedef entry */
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* energyCalibration */
static void G__setup_memvarenergyCalibration(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration));
   { energyCalibration *p; p=(energyCalibration*)0x1000; if (p) { }
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibrationFunctions),-1,-1,4,"funcs=",0,(char*)NULL);
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,4,"plot_flag=",0,"*< 0 does not plot, >0 plots (flags?) */");
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,4,"cs_flag=",0,"*< 0 functions without charge sharing contribution, >0 with charge sharing contribution */");
   }
   G__tag_memvar_reset();
}


   /* angularCalibration */
static void G__setup_memvarangularCalibration(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration));
   { angularCalibration *p; p=(angularCalibration*)0x1000; if (p) { }
   G__memvar_setup((void*)0,105,0,0,-1,-1,-1,4,"direction=",0,"*< angulat direction of the detector -can be +1 or -1 */");
   G__memvar_setup((void*)0,102,0,0,-1,-1,-1,4,"encoder=",0,"*< position of the detector encoder */");
   G__memvar_setup((void*)0,102,0,0,-1,-1,-1,4,"totalOffset=",0,"*< total offset of the detector */");
   G__memvar_setup((void*)0,102,0,0,-1,-1,-1,4,"ang_min=",0,"*< minimum of the angular range for peak fitting*/");
   G__memvar_setup((void*)0,102,0,0,-1,-1,-1,4,"ang_max=",0,"*< maximum of the angular range for peak fitting */");
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarTSlsDetectorDict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncenergyCalibration(void) {
   /* energyCalibration */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration));
   G__memfunc_setup("energyCalibration",1778,G__TSlsDetectorDict_85_0_1, 105, G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setPlotFlag",1125,G__TSlsDetectorDict_85_0_2, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 '-1' p", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setScanSign",1122,G__TSlsDetectorDict_85_0_3, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 '0' s", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setChargeSharing",1634,G__TSlsDetectorDict_85_0_4, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 '-1' p", (char*)NULL, (void*) NULL, 0);
   // automatic copy constructor
   G__memfunc_setup("energyCalibration", 1778, G__TSlsDetectorDict_85_0_5, (int) ('i'), G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration), -1, 0, 1, 1, 1, 0, "u 'energyCalibration' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~energyCalibration", 1904, G__TSlsDetectorDict_85_0_6, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__TSlsDetectorDict_85_0_7, (int) ('u'), G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration), -1, 1, 1, 1, 1, 0, "u 'energyCalibration' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncangularCalibration(void) {
   /* angularCalibration */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration));
   G__memfunc_setup("angularCalibration",1874,G__TSlsDetectorDict_87_0_1, 105, G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setDirection",1261,G__TSlsDetectorDict_87_0_2, 105, -1, -1, 0, 1, 1, 1, 0, "i - - 0 '0' d", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setEncoder",1036,G__TSlsDetectorDict_87_0_3, 102, -1, -1, 0, 1, 1, 1, 0, "f - - 0 - f", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("getEncoder",1024,G__TSlsDetectorDict_87_0_4, 102, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setTotalOffset",1463,G__TSlsDetectorDict_87_0_5, 102, -1, -1, 0, 1, 1, 1, 0, "f - - 0 - f", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("getTotalOffset",1451,G__TSlsDetectorDict_87_0_6, 102, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("setAngularRange",1539,G__TSlsDetectorDict_87_0_7, 121, -1, -1, 0, 2, 1, 1, 0, 
"f - - 0 - mi f - - 0 - ma", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("getAngularRange",1527,G__TSlsDetectorDict_87_0_8, 121, -1, -1, 0, 2, 1, 1, 0, 
"f - - 1 - mi f - - 1 - ma", (char*)NULL, (void*) NULL, 0);
   // automatic copy constructor
   G__memfunc_setup("angularCalibration", 1874, G__TSlsDetectorDict_87_0_9, (int) ('i'), G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration), -1, 0, 1, 1, 1, 0, "u 'angularCalibration' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~angularCalibration", 2000, G__TSlsDetectorDict_87_0_10, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__TSlsDetectorDict_87_0_11, (int) ('u'), G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration), -1, 1, 1, 1, 1, 0, "u 'angularCalibration' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncTSlsDetectorDict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();


   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalTSlsDetectorDict() {
  G__cpp_setup_global0();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcTSlsDetectorDict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__TSlsDetectorDictLN_energyCalibrationFunctions = { "energyCalibrationFunctions" , 99 , -1 };
G__linked_taginfo G__TSlsDetectorDictLN_energyCalibration = { "energyCalibration" , 99 , -1 };
G__linked_taginfo G__TSlsDetectorDictLN_angularCalibration = { "angularCalibration" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableTSlsDetectorDict() {
  G__TSlsDetectorDictLN_energyCalibrationFunctions.tagnum = -1 ;
  G__TSlsDetectorDictLN_energyCalibration.tagnum = -1 ;
  G__TSlsDetectorDictLN_angularCalibration.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableTSlsDetectorDict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__TSlsDetectorDictLN_energyCalibrationFunctions);
   G__tagtable_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_energyCalibration),sizeof(energyCalibration),-1,1280,(char*)NULL,G__setup_memvarenergyCalibration,G__setup_memfuncenergyCalibration);
   G__tagtable_setup(G__get_linked_tagnum(&G__TSlsDetectorDictLN_angularCalibration),sizeof(angularCalibration),-1,1280,(char*)NULL,G__setup_memvarangularCalibration,G__setup_memfuncangularCalibration);
}
extern "C" void G__cpp_setupTSlsDetectorDict(void) {
  G__check_setup_version(30051515,"G__cpp_setupTSlsDetectorDict()");
  G__set_cpp_environmentTSlsDetectorDict();
  G__cpp_setup_tagtableTSlsDetectorDict();

  G__cpp_setup_inheritanceTSlsDetectorDict();

  G__cpp_setup_typetableTSlsDetectorDict();

  G__cpp_setup_memvarTSlsDetectorDict();

  G__cpp_setup_memfuncTSlsDetectorDict();
  G__cpp_setup_globalTSlsDetectorDict();
  G__cpp_setup_funcTSlsDetectorDict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncTSlsDetectorDict();
  return;
}
class G__cpp_setup_initTSlsDetectorDict {
  public:
    G__cpp_setup_initTSlsDetectorDict() { G__add_setup_func("TSlsDetectorDict",(G__incsetup)(&G__cpp_setupTSlsDetectorDict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initTSlsDetectorDict() { G__remove_setup_func("TSlsDetectorDict"); }
};
G__cpp_setup_initTSlsDetectorDict G__cpp_setup_initializerTSlsDetectorDict;

