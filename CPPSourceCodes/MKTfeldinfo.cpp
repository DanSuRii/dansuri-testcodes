#include <iostream>
#include <map>
#include <vector>
#include <memory>

struct MARKANT_FELD
{

};

template<class Owner, int ETyp>
struct IMPL_FELD : public MARKANT_FELD
{

};

typedef std::shared_ptr< MARKANT_FELD > FeldTyp;
typedef std::vector<FeldTyp> CONT_FELD;


struct BASE_MARKANT_FELDINFO
{
/* 
	virtual const MARKANT_FELD& GetFled() const = 0;
	virtual size_t GetFeldsCnt() const = 0;
	virtual size_t GetFieldsSize() const = 0 ;
 */
    virtual const CONT_FELD& GetInfoCont()  = 0;
    //virtual const char* GetName() const = 0;
};

typedef int NUM_3CC;


template< class Derived >
struct IMPL_MKT_FELDINFO_COMN_INTERFACE : public BASE_MARKANT_FELDINFO
{
	typedef Derived TDerived;
	// Check Derived equal Sametype derived... must in here
	// static_assert( Derived::Base::Derived == Derived )
	// static_assert( decltype() );

/*
    const CONT_FELD& GetInfoContImpl() 
    {
        return static_cast<Derived*>(this)->_cont;
    }
*/

    virtual const CONT_FELD& GetInfoCont() 
    {
        // return GetInfoContImpl();

        return static_cast<Derived*>(this)->_cont;
        
    }
};

// without implement impossible to Instancenation
// Prevent Instancenation without implement
template<NUM_3CC> 
struct IMPL_MARKANT_FELDINFO
{
    
};


template<  >
struct IMPL_MARKANT_FELDINFO<'ZAK'> : public IMPL_MKT_FELDINFO_COMN_INTERFACE<IMPL_MARKANT_FELDINFO<'ZAK'>>
{

	enum : int { EFELDS_NULL, ESATART, BEARBETIUNGS_WOCHE, EFELDS_CNT };
	enum{  };

	// std::vector< FeldTyp > _cont = std::vector< FeldTyp >( EFELDS_CNT );
	//to be Generate, Multithread - Synchronous - Concurrency mode?
	//just give a Handle, lock - release automatation.
	/*******************************
	 * 	 struct Handler{
	 * 		 Handler(){ locker->lock }
	 * 		 ~Handler(){ locker->release }
	 * 		TypCont _cont;
	 *  } GetCont()
	*/
	static CONT_FELD _cont;
    
};

	template< typename FELDINFO, unsigned int n >
	struct UTIL_ACCM_LEN
	{
		enum{ LEN = IMPL_FELD< FELDINFO, n >::MAXLEN + UTIL_ACCM_LEN<FELDINFO ,n-1 >::LEN};
	};

	template<typename FELDINFO>
	struct UTIL_ACCM_LEN<FELDINFO,0>
	{
		enum{ LEN = 0 };
	};

namespace NS_ZAK
{
	typedef IMPL_MARKANT_FELDINFO<'ZAK'> FELDINFO;
    typedef UTIL_ACCM_LEN<FELDINFO, FELDINFO::EFELDS_CNT> ACCM_LEN_MAX;

	template<>
	struct IMPL_FELD< FELDINFO ,FELDINFO::ESATART > : public MARKANT_FELD
	{
		enum { MAXLEN = 3 };
	};

	template<>
	struct IMPL_FELD< FELDINFO, FELDINFO::BEARBETIUNGS_WOCHE > : public MARKANT_FELD
	{
		enum { MAXLEN = 6 };
	};

	template<>
	struct IMPL_FELD< FELDINFO, FELDINFO::EFELDS_CNT> : public MARKANT_FELD
	{
		enum { MAXLEN = 0 };
	};

}

CONT_FELD NS_ZAK::FELDINFO::_cont {
    std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::EFELDS_NULL > >( ),

    std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::ESATART > >(  ),
    std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::BEARBETIUNGS_WOCHE > >(  ),

    std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::EFELDS_CNT > >(  ),    
};
/*
*/

// 어짜피 테스트 모듈이니까, 같은 벡터에 두개씩 넣어도 상관없다.
template< typename FELDINFO ,int nCur>
struct Register
{

};


//template<>

/*
template <>
struct IMPL_FELD <	IMPL_MARKANT_FELDINFO<'ZAK'>
	, IMPL_MARKANT_FELDINFO<'ZAK'>::EFELDS_NULL >
{
	enum { LEN = 0 };
};


template <>
struct IMPL_FELD <	IMPL_MARKANT_FELDINFO<'ZAK'>
					, IMPL_MARKANT_FELDINFO<'ZAK'>::EFELDS_CNT >
{
	enum { LEN = IMPL_FELD< Owner, ETyp-1 >::LEN };
};
*/


//template<>

struct BASE_MARKANT_DATA
{

};

template<NUM_3CC n3cc>
struct IMPL_MARKANT_DATA : public BASE_MARKANT_DATA
{
	
	std::vector< std::shared_ptr<BASE_MARKANT_FELDINFO> > contFeldInfo;
		IMPL_MARKANT_FELDINFO<n3cc>  feldInfo;

};

class MARKANT_READER
{
	std::map< NUM_3CC, std::shared_ptr<BASE_MARKANT_FELDINFO> > _contFeldinfo;

public:
	MARKANT_READER();
};



/**/

MARKANT_READER::MARKANT_READER()
	:
	_contFeldinfo{ 
	//std::make_pair('ZAK', std::make_shared< BASE_MARKANT_DATA >()),
	std::make_pair('ZAK', std::make_shared< IMPL_MARKANT_FELDINFO<'ZAK'> >()),
	// { 'ZBA', std::make_shared< IMPL_MARKANT_DATA<'ZBA'> >() } ,
	}
    /*
    */	
{
	//_contFeldinfo['ZAK'] = std::make_shared< BASE_MARKANT_DATA >;
	// _contFeldinfo['ZAK'] = std::make_shared< IMPL_MARKANT_FELDINFO<'ZAK'> >();

    
    for( auto feld : _contFeldinfo)
    {
        std::cout << feld.first << std::endl;

        const CONT_FELD& infoCont = feld.second->GetInfoCont();
        // infoCont.push_back( std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::ESATART > >(  ) );
        for( auto it : infoCont)
        {
            // it->
        }
    }

}

int main(void)
{
    // FeldTyp pData = std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::ESATART > >(  );

/*
    NS_ZAK::FELDINFO::_cont.push_back(
        std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::EFELDS_NULL > >( )
    );

    NS_ZAK::FELDINFO::_cont.push_back(
        std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::ESATART > >(  )
    );
    NS_ZAK::FELDINFO::_cont.push_back(
        std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::BEARBETIUNGS_WOCHE > >(  )
    );

    NS_ZAK::FELDINFO::_cont.push_back(
        std::make_shared< IMPL_FELD< NS_ZAK::FELDINFO, NS_ZAK::FELDINFO::EFELDS_CNT > >(  )    
    );
*/


    // int nLen = NS_ZAK::UTIL_ACCM_LEN< NS_ZAK::FELDINFO::EFELDS_CNT >::LEN;
    int nLen = NS_ZAK::ACCM_LEN_MAX::LEN;
    

    std::cout << nLen << std::endl;
    // std::cout << 

    MARKANT_READER reader;

    return 0;
}