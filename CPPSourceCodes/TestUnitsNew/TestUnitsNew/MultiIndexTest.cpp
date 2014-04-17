#include "stdafx.h"
#include "Work.h"

#define BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE 5 
#define BOOST_MULTI_INDEX_LIMIT_TAG_SIZE 3 
#define BOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE 5 
#include <mic_visualizer.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost;
using namespace boost::multi_index;

struct employee
{
	int         id;
	std::string name;

	employee(int id,const std::string& name):id(id),name(name){}

	bool operator<(const employee& e)const{return id<e.id;}
};

std::ostream& operator<<( std::ostream& ostr, const employee& rhs )
{
	return ostr << rhs.id << ".[" << rhs.name << "]";
}

// define a multiply indexed set with indices by id and name
typedef multi_index_container<
	employee,
	indexed_by<
	// sort by employee::operator<
	ordered_unique<identity<employee> >,

	// sort by less<string> on name
	ordered_non_unique<member<employee,std::string,&employee::name> >
	> 
> employee_set;
VISUALIZE_MULTI_INDEX_CONTAINER(employee_set)

void print_out_by_name(const employee_set& es)
{
	// get a view to index #1 (name)
	const employee_set::nth_index<1>::type& name_index=es.get<1>();
	// use name_index as a regular std::set
	std::copy(
		name_index.begin(),name_index.end(),
		std::ostream_iterator<employee>(std::cout,"|"));

/*
	for each( auto iter in name_index )
	{
		std::cout << iter.id << iter.name;
	}
*/
}
#include <boost/mpl/string.hpp>  
#include <boost/mpl/fold.hpp>  
#include <boost/mpl/size_t.hpp>  
#include <boost/functional/hash.hpp>
	namespace meta  
{  
#pragma warning(push)  
	// disable addition overflow warning  
#pragma warning(disable:4307)  

	template <typename Seed, typename Value>  
	struct hash_combine  
	{  
		typedef mpl::size_t<  
			Seed::value ^ (static_cast<std::size_t>(Value::value)  
			+ 0x9e3779b9 + (Seed::value << 6) + (Seed::value >> 2))  
		> type;  
	};  

#pragma warning(pop)  

	// Hash any sequence of integral wrapper types  
	template <typename Sequence>  
	struct hash_sequence  
		: mpl::fold<  
		Sequence  
		, mpl::size_t<0>  
		, hash_combine<mpl::_1, mpl::_2>  
		>::type  
	{};  

	// For hashing std::strings et al that don't include the zero-terminator  
	template <typename String>  
	struct hash_string  
		: hash_sequence<String>  
	{};  

	// Hash including terminating zero for char arrays  
	template <typename String>  
	struct hash_cstring  
		: hash_combine<  
		hash_sequence<String>  
		, mpl::size_t<0>  
		>::type  
	{};  

} // namespace meta  
void WorkImpl<MultiIndexTest>::DoWork()
{
	employee_set es;
	es.insert( employee(1, "denny") );
	es.insert( employee(2, "annie") );
	
	print_out_by_name(es);

	std::string strValue("bunnies");
	
	switch (boost::hash_value(strValue))  
	{  
	case meta::hash_cstring< mpl::string<'bunn','ies'> >::value:  
		std::cout << "bunnies!\n";  
		break;  
	case meta::hash_cstring< mpl::string<'bear','s'> >::value:  
		std::cout << "bears!\n";  
		break;  
	default:  
		std::cout << "no hash matched\n";  
	}  
}


