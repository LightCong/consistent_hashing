#include <iostream>
#include <string>
#include<functional>
#include<cassert>
#include "consistent_hashing.hpp"



int hash_calc(Vnode<int>*vnode)
{
    vnode->set_hash_val(vnode->get_virtual_id());
    return vnode->get_hash_val();
}

int hash_calc_str(Vnode<std::string>*vnode)
{
    std::hash<std::string> hash_func=std::hash<std::string>();
    vnode->set_hash_val(hash_func(vnode->get_virtual_id()));
}


void test_create_vnode()
{
    // test create vnode

    // case one
    Vnode<int> v1=Vnode<int>(1,2,hash_calc);
    assert(v1.get_hash_val()==1);

    // case two
    Vnode<std::string>v2=Vnode<std::string>("aa","b",hash_calc_str);

    assert(v2.get_virtual_id()=="aa");
    assert(v2.get_hash_val()==63151013);

    std::cout<<"test_create_vnode success!"<<std::endl;
}


void test_add_and_del_vnode()
{
    ConsistentHashingMap<int> c_hash_map=ConsistentHashingMap<int>(hash_calc);


    // test add_vnode
    c_hash_map.add_vnode(0,0);
    c_hash_map.add_vnode(1,0);
    c_hash_map.add_vnode(2,0);
    c_hash_map.add_vnode(3,1);
    c_hash_map.add_vnode(4,1);
    c_hash_map.add_vnode(5,1);

#ifdef DEBUG
    assert(c_hash_map.hash_map.size()==6);
#endif

    c_hash_map.del_vnode_of_virtual_id(1);

#ifdef DEBUG
    assert(c_hash_map.hash_map.size()==5);
#endif


    c_hash_map.del_vnode_of_real_id(1);

#ifdef DEBUG
    assert(c_hash_map.hash_map.size()==2);
#endif

    // todo del hash test

    c_hash_map.del_vnode(0);

#ifdef DEBUG
    assert(c_hash_map.hash_map.size()==1);
#endif

    std::cout<<"test_add_and_del_vnode success!"<<std::endl;

}

void test_find()
{
    ConsistentHashingMap<int> c_hash_map=ConsistentHashingMap<int>(hash_calc);

    c_hash_map.add_vnode(0,0);
    c_hash_map.add_vnode(100,1);
    c_hash_map.add_vnode(200,2);

    int real_id=-1;
    c_hash_map.find_real_id_of_key(0,&real_id);
    assert(real_id==0);

    c_hash_map.find_real_id_of_key(100,&real_id);
    assert(real_id==1);

    c_hash_map.find_real_id_of_key(200,&real_id);
    assert(real_id==2);

    c_hash_map.find_real_id_of_key(50,&real_id);
    assert(real_id==1);

    c_hash_map.find_real_id_of_key(150,&real_id);
    assert(real_id==2);

    c_hash_map.find_real_id_of_key(250,&real_id);
    assert(real_id==0);

    c_hash_map.add_vnode(160,3);
    c_hash_map.find_real_id_of_key(150,&real_id);
    assert(real_id==3);


    c_hash_map.del_vnode_of_real_id(3);
    c_hash_map.find_real_id_of_key(150,&real_id);
    assert(real_id==2);


    std::cout<<"test_find success!"<<std::endl;
}



int main() {

    test_create_vnode();
    test_add_and_del_vnode();
    test_find();

    return 0;
}