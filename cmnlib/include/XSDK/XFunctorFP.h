
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _FUNCTOR_FP_H_
#define _FUNCTOR_FP_H_

namespace XSDK
{

    //This set of interfaces/classes allow you to easily make
    //pointers to member functions of a certain class (for callbacks, etc...)
    //
    //They allow up to (can be extended) five arguments as of right now
    //
    //They allow different return types as well as different argument types
    //through the use of templates
    //
    //Examples of these classes and their interfaces can be found below
    //
    //More extended exmaples can be found in the unit tests
    //
    //Example class for reference
    //class TestClass
    //{
    //public:
    //    unsigned Print1(string s1)
    //    {
    //        printf("First: %s\n",s1.c_str());
    //        return s1.size();
    //    }
    //    unsigned Print2(string s1,string s2)
    //    {
    //        printf("First: %s\n",s1.c_str());
    //        printf("Second: %s\n",s2.c_str());
    //        return s1.size()+s2.size();
    //    }
    //    unsigned Print3(string s1,string s2,string s3)
    //    {
    //        printf("First: %s\n",s1.c_str());
    //        printf("Second: %s\n",s2.c_str());
    //        printf("Third: %s\n",s3.c_str());
    //        return s1.size()+s2.size()+s3.size();
    //    }
    //    unsigned Print4(string s1,string s2,string s3 string s4)
    //    {
    //        printf("First: %s\n",s1.c_str());
    //        printf("Second: %s\n",s1.c_str());
    //        printf("Third: %s\n",s1.c_str());
    //        printf("Fourth: %s\n",s1.c_str());
    //        return s1.size()+s2.size()+s3.size()+s4.size();
    //    }
    //    unsigned Print5(string s1,string s2,string s3,string s4,string s5)
    //    {
    //        printf("First: %s\n",s1.c_str());
    //        printf("Second: %s\n",s2.c_str());
    //        printf("Third: %s\n",s3.c_str());
    //        printf("Fourth: %s\n",s4.c_str());
    //        printf("Fifth: %s\n",s5.c_str());
    //        return s1.size()+s2.size()+s3.size()+s4.size()+s5.size();
    //    }
    //};

    //Functor function pointer with a single argument
    //Interface
    template<class ReturnType,class Arg1>
    class IXFunctorFP1
    {
    public:
        virtual ReturnType operator()(Arg1) = 0;
    };

    //Implementation
    template <class TheClass,class ReturnType,class Arg1>
    class XFunctorFP1 : public IXFunctorFP1<ReturnType,Arg1>
    {
    private:
        ReturnType (TheClass::*_fpt)(Arg1);
        TheClass * _obj;
    public:
        XFunctorFP1(TheClass * obj, ReturnType (TheClass::*fpt)(Arg1))
        {
            _obj = obj;
            _fpt = fpt;
        };

        virtual ReturnType operator()(Arg1 arg)
        {
            return (*_obj.*_fpt)(arg);
        };
    };
    //Now we want a pointer to member function, "Print1", which object "a" contains
    //Example usage
    //main()
    //{
    //    TestClass a;
    //
    //    //Heap or stack based allocation
    //    //These examples declare the functors on the heap
    //    //And the objects they use ("a") on the stack
    //    IXFunctorFP1<unsigned,string> * fFp1 =
    //      new XFunctorFP1<TestClass,unsigned,string>(&a,&TestClass::Print1);
    //
    //    unsigned size = (*fFp1)("A String");
    //
    //    printf("%u\n",size);
    //
    //    delete fFp1;
    //}
    //////////


    //Functor function pointer with two arguments
    //Interface
    template<class ReturnType,class Arg1,class Arg2>
    class IXFunctorFP2
    {
    public:
        virtual ReturnType operator()(Arg1,Arg2) = 0;
    };

    //Implementation
    template <class TheClass,class ReturnType,class Arg1,class Arg2>
    class XFunctorFP2 : public IXFunctorFP2<ReturnType,Arg1,Arg2>
    {
    private:
        ReturnType (TheClass::*_fpt)(Arg1,Arg2);
        TheClass * _obj;
    public:
        XFunctorFP2(TheClass * obj, ReturnType (TheClass::*fpt)(Arg1,Arg2))
        {
            _obj = obj;
            _fpt = fpt;
        };
        virtual ReturnType operator()(Arg1 arg,Arg2 arg2)
        {
            return (*_obj.*_fpt)(arg,arg2);
        };
    };
    //Now we want a pointer to member function, "Print2", which object "a" contains
    //Example usage
    //main()
    //{
    //    TestClass a;
    //
    //    IXFunctorFP2<unsigned,string,string> * fFp2 =
    //          new XFunctorFP2<TestClass,unsigned,string,string>(&a,&TestClass::Print2);
    //
    //    unsigned size = (*fFp2)("A String","A String 2");
    //
    //    printf("%u\n",size);
    //
    //    delete fFp2;
    //}
    ///////////


    //Functor function pointer with three arguments
    //Interface
    template<class ReturnType,class Arg1,class Arg2,class Arg3>
    class IXFunctorFP3
    {
    public:
        virtual ReturnType operator()(Arg1,Arg2,Arg3) = 0;
    };

    //Implementation
    template <class TheClass,class ReturnType,class Arg1,class Arg2,
              class Arg3>
    class XFunctorFP3 : public IXFunctorFP3<ReturnType,Arg1,Arg2,Arg3>
    {
    private:
        ReturnType (TheClass::*_fpt)(Arg1,Arg2,Arg3);
        TheClass * _obj;
    public:
        XFunctorFP3(TheClass * obj,ReturnType (TheClass::*fpt)(Arg1,Arg2,Arg3))
        {
            _obj = obj;
            _fpt = fpt;
        };
        virtual ReturnType operator()(Arg1 arg,Arg2 arg2,Arg3 arg3)
        {
            return (*_obj.*_fpt)(arg,arg2,arg3);
        };
    };
    //Now we want a pointer to member function, "Print3", which object "a" contains
    //Example usage
    //main()
    //{
    //    TestClass a;
    //
    //    IXFunctorFP3<unsigned,string,string,string> * fFp3 =
    //      new XFunctorFP3<TestClass,unsigned,string,string,string>(&a,&TestClass::Print3);
    //
    //    unsigned size = (*fFp3)("A String","A String 2","A String 3");
    //
    //    printf("%u\n",size);
    //
    //    delete fFp3;
    //}
    //////////////


    //Functor function pointer with four arguments
    //Interface
    template<class ReturnType,class Arg1,class Arg2,class Arg3,class Arg4>
    class IXFunctorFP4
    {
    public:
        virtual ReturnType operator()(Arg1,Arg2,Arg3,Arg4) = 0;
    };

    //Implementation
    template <class TheClass,class ReturnType,class Arg1,class Arg2,
              class Arg3,class Arg4>
    class XFunctorFP4 : public IXFunctorFP4<ReturnType,Arg1,Arg2,Arg3,Arg4>
    {
    private:
        ReturnType (TheClass::*_fpt)(Arg1,Arg2,Arg3,Arg4);
        TheClass * _obj;
    public:
        XFunctorFP4(TheClass * obj,ReturnType (TheClass::*fpt)(Arg1,Arg2,Arg3,Arg4))
        {
            _obj = obj;
            _fpt = fpt;
        };
        virtual ReturnType operator()(Arg1 arg,Arg2 arg2,Arg3 arg3,Arg4 arg4)
        {
            return (*_obj.*_fpt)(arg,arg2,arg3,arg4);
        };
    };
    //Now we want a pointer to member function, "Print4", which object "a" contains
    //Example usage
    //main()
    //{
    //    TestClass a;
    //
    //    IXFunctorFP4<unsigned,string,string,string,string> * fFp4 =
    //      new XFunctorFP3<TestClass,unsigned,string,string,string,string>(&a,&TestClass::Print4);
    //
    //    unsigned size = (*fFp4)("A String","A String 2","A String 3","A String 4");
    //
    //    printf("%u\n",size);
    //
    //    delete fFp4;
    //}
    ////////////


    //Functor function pointer with five arguments
    //Interface
    template<class ReturnType,class Arg1,class Arg2,
             class Arg3,class Arg4,class Arg5>
    class IXFunctorFP5
    {
    public:
        virtual ReturnType operator()(Arg1,Arg2,Arg3,Arg4,Arg5) = 0;
    };

    //Implementation
    template <class TheClass,class ReturnType,class Arg1,class Arg2,
              class Arg3,class Arg4,class Arg5>
    class XFunctorFP5 : public IXFunctorFP5<ReturnType,Arg1,Arg2,Arg3,Arg4,Arg5>
    {
    private:
        ReturnType (TheClass::*_fpt)(Arg1,Arg2,Arg3,Arg4,Arg5);
        TheClass * _obj;
    public:
        XFunctorFP5(TheClass * obj,ReturnType (TheClass::*fpt)(Arg1,Arg2,Arg3,Arg4,Arg5))
        {
            _obj = obj;
            _fpt = fpt;
        };
        virtual ReturnType operator()(Arg1 arg,Arg2 arg2,Arg3 arg3,Arg4 arg4,Arg5 arg5)
        {
            return (*_obj.*_fpt)(arg,arg2,arg3,arg4,arg5);
        };
    };
    //Now we want a pointer to member function, "Print5", which object "a" contains
    //Example usage
    //main()
    //{
    //    TestClass a;
    //
    //    IXFunctorFP5<unsigned,string,string,string,string,string> * fFp5 =
    //      new XFunctorFP3<TestClass,unsigned,string,string,string,string,string>(&a,&TestClass::Print5);
    //
    //    unsigned size = (*fFp5)("A String","A String 2","A String 3","A String 4","A String 5");
    //
    //    printf("%u\n",size);
    //
    //    delete fFp5;
    //}
    ///////////

};

#endif
