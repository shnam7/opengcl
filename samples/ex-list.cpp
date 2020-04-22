//
//		* exlist.cpp
//
//		OpenGCL Example: glist operations
//
//		Written by Soo-Hyuk Nam.
//			2004. 10. 11. Mon.
//

#include <glist.h>
#include <stdio.h>
#include <memory.h>


class Session {
protected:

public:
	xlist_head		node;
	int				val;
	Session();
    ~Session();
	int set(int v);
};

Session::Session()
{
	node.obj = this;
}

Session::~Session()
{
}

int	Session::set(int v)
{
	val = v;
	return 0;
}

int main()
{
	xlist_head list;
	//INIT_LIST_HEAD(&list);
	Session s1, s2, s3;
	s1.set(1);s2.set(2);s3.set(3);
	list_add_tail(&s1.node, &list);
	list_add_tail(&s2.node, &list);
	list_add_tail(&s3.node, &list);

	/*
	list_head *pos;
	list_for_each(pos, &list)
	{
		Session *ss = xlist_entry(pos, Session);
		printf( "ss: %d\n", ss->val );
	}
	*/
	list_head *pos, *tmp;
	list_for_each_safe(pos, tmp, &list)
	{
		//Session *ss = list_entry(pos, Session, node);
		Session *ss = xlist_entry(pos, Session);
		printf( "ss: %d\n", ss->val );
	}
	return 0;
}
