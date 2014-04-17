#include "stdafx.h"
#include "Work.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <utility>
#include <boost/foreach.hpp>
#include <string>

//enum /*class */Event { RED, GREEN, BLUE, ORANGE, MAGENTA, CYAN };

//subject가 아니라, collection 이 되어야 한다.\
//event 타입이, subject를 정의하는 요소가 되어선 안되기 때문.

//KEY타입이 여러가지가 있을 때 , 여러 event타입에 대응하는 이벤트 핸들러의 등록 / 여러 event타입에 대응하는 동작을 구현 할 수 있는가?


/*
pseudo code

class ObserverInterface
	~ObserverInterface()
	{
		if(observerCollector) observerCollector->RemoveME( GetObsID() );
	}


class observerCollector
	RegisterID()
	~observerCollector()
	{
		for( auto obs : obsContainers )
			obs->CollectorDestroid(this)
	}
	registerEvt(Evt, OBSERVER_OPERATOR obsOP)
	{
		observers[evet].push_back(  obs, obsOP );
	}
	unregisterEvt(Evt, OBSERVER obs)
	{
		
	}

	unregisterEvt(OBSERVER obs)
	{
	}


class ObserverImplA : public ObserverInterface;

class ObserverImplB : public ObserverInterface;

*/

class SubjectB
{

	class ObserverCollectionInterface
	{
	};
	
	template<typename Event>
	class ObserverCollection : public ObserverCollectionInterface
	{
	public:
		std::map<Event, std::vector< std::function< void() > > > observers_;
	};
	

	template< typename Event, typename Observer >	
	void registerObserver(const Event& event, Observer&& observer)
	{
		GetObserverCollection<Event>().registerObserver( event, observer );
	}

	
	template<typename Event>
	ObserverCollectionInterface& GetObserverCollection( );

	template<>	ObserverCollectionInterface& GetObserverCollection<std::string>(){ return m_strObserverCollection; }
	//template<>	ObserverCollectionInterface& GetObserverCollection<std::string>(){ return m_strObserverCollection; }


	//{
	//	//subject 소유의 observer collection을 꺼내줘야 한다.
	//}

	ObserverCollection<std::string> m_strObserverCollection;
};

template <typename Event>
class Subject
{
public:
	template <typename Observer>
	void registerObserver(const Event& event, Observer&& observer)
	{
		observers_[ event ].push_back(std::forward<Observer>(observer));
	}

	template <typename Observer>
	void registerObserver(Event&& event, Observer&& observer)
	{
		observers_[std::move(event)].push_back(std::forward<Observer>(observer));
	}

	void notify(const Event& event)
	{
		BOOST_FOREACH( const auto& obs, observers_.at(event) )
			obs();
	}

private:
	std::map<Event, std::vector< std::function< void() > > > observers_;
};

void foo(int i)
{
	std::cout << "foo( " <<  i << " )\n";
}

void bar() {
	std::cout << "bar()\n";
}

void WorkImpl<ObserverGeneric>::DoWork()
{
	Subject<std::string> s;
	s.registerObserver( std::string("GREEN"), &bar);
	s.registerObserver("ORANGE", std::bind(foo, 42));
	s.registerObserver("RED", std::bind(foo, 12345));

	std::string msg("Hello, RED!");
	s.registerObserver("RED", [&msg]()->void{std::cout << msg.c_str() << std::endl;});

	s.notify("GREEN");
	s.notify("RED");
	s.notify("ORANGE");
}
