// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictmyLorentzVector

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "myLorentzVector.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_myLorentzVector(void *p = 0);
   static void *newArray_myLorentzVector(Long_t size, void *p);
   static void delete_myLorentzVector(void *p);
   static void deleteArray_myLorentzVector(void *p);
   static void destruct_myLorentzVector(void *p);
   static void streamer_myLorentzVector(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myLorentzVector*)
   {
      ::myLorentzVector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myLorentzVector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("myLorentzVector", ::myLorentzVector::Class_Version(), "myLorentzVector.h", 12,
                  typeid(::myLorentzVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myLorentzVector::Dictionary, isa_proxy, 16,
                  sizeof(::myLorentzVector) );
      instance.SetNew(&new_myLorentzVector);
      instance.SetNewArray(&newArray_myLorentzVector);
      instance.SetDelete(&delete_myLorentzVector);
      instance.SetDeleteArray(&deleteArray_myLorentzVector);
      instance.SetDestructor(&destruct_myLorentzVector);
      instance.SetStreamerFunc(&streamer_myLorentzVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myLorentzVector*)
   {
      return GenerateInitInstanceLocal((::myLorentzVector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::myLorentzVector*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr myLorentzVector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *myLorentzVector::Class_Name()
{
   return "myLorentzVector";
}

//______________________________________________________________________________
const char *myLorentzVector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myLorentzVector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int myLorentzVector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myLorentzVector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myLorentzVector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myLorentzVector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myLorentzVector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myLorentzVector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void myLorentzVector::Streamer(TBuffer &R__b)
{
   // Stream an object of class myLorentzVector.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TLorentzVector::Streamer(R__b);
      {
         vector<int> &R__stl =  tracks;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, myLorentzVector::IsA());
   } else {
      R__c = R__b.WriteVersion(myLorentzVector::IsA(), kTRUE);
      TLorentzVector::Streamer(R__b);
      {
         vector<int> &R__stl =  tracks;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myLorentzVector(void *p) {
      return  p ? new(p) ::myLorentzVector : new ::myLorentzVector;
   }
   static void *newArray_myLorentzVector(Long_t nElements, void *p) {
      return p ? new(p) ::myLorentzVector[nElements] : new ::myLorentzVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_myLorentzVector(void *p) {
      delete ((::myLorentzVector*)p);
   }
   static void deleteArray_myLorentzVector(void *p) {
      delete [] ((::myLorentzVector*)p);
   }
   static void destruct_myLorentzVector(void *p) {
      typedef ::myLorentzVector current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_myLorentzVector(TBuffer &buf, void *obj) {
      ((::myLorentzVector*)obj)->::myLorentzVector::Streamer(buf);
   }
} // end of namespace ROOT for class ::myLorentzVector

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 450,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace {
  void TriggerDictionaryInitialization_dictmyLorentzVector_Impl() {
    static const char* headers[] = {
"myLorentzVector.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/Cellar/root/6.12.06/include/root",
"/Users/imoo/ddc_matched/temp_newclass/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictmyLorentzVector dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(the class title)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(the class title)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(the class title)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(the class title)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$myLorentzVector.h")))  myLorentzVector;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictmyLorentzVector dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "myLorentzVector.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"myLorentzVector", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictmyLorentzVector",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictmyLorentzVector_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictmyLorentzVector_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictmyLorentzVector() {
  TriggerDictionaryInitialization_dictmyLorentzVector_Impl();
}
