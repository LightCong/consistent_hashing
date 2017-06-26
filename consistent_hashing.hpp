//
// Created by dacuobi on 17/6/22.
//

#ifndef CONSISTENT_HASHING_CONSISTENT_HASHING_HPP
#define CONSISTENT_HASHING_CONSISTENT_HASHING_HPP
#include<map>
#include<iostream>

template<typename id_type>
class Vnode
{
public:

    typedef std::function<int (Vnode<id_type>*)> outer_func_type;

    Vnode(id_type virtual_id,id_type real_id,outer_func_type func):
            virtual_id(virtual_id),
            real_id(real_id)
    {
        hash_calc_func=std::bind(func,this);
        hash_calc_func();
    }

    id_type get_virtual_id()
    {
        return virtual_id;
    }

    id_type get_real_id()
    {
        return real_id;
    }

    int get_hash_val()
    {
        return hash_val;
    }

    void set_hash_val(int hash_val)
    {
        this->hash_val=hash_val;
    }

private:
    std::function<int ()> hash_calc_func;
    id_type virtual_id;
    id_type real_id;
    int hash_val;
};

template<typename id_type>
class ConsistentHashingMap
{

    typedef std::function<int (Vnode<id_type>*)> outer_func_type;

    typedef typename std::map<int,std::shared_ptr<Vnode<id_type>>>::iterator iter_type;

public:
    ConsistentHashingMap(outer_func_type hash_func):
        hash_func(hash_func)
    {}


    bool add_vnode(id_type virtual_id,id_type real_id)
    {
        // 添加一个节点

        std::shared_ptr<Vnode<id_type>> node_ptr(new Vnode<id_type>(virtual_id,real_id,hash_func));

        //判断是否存在
        iter_type iter= hash_map.find(node_ptr->get_hash_val());

        if(iter!=hash_map.end()) //已经插入这样一个虚拟节点了
            return false;

        hash_map[node_ptr->get_hash_val()]=node_ptr;
        return true;
    }


    bool del_vnode(int hash_val)
    {
        //判断是否存在
        iter_type iter= hash_map.find(hash_val);

        if(iter==hash_map.end()) // 不存在
            return false;

        hash_map.erase(hash_val);
    }

    bool del_vnode_of_virtual_id(id_type virtual_id)
    {
        //删除指定virtual_id的虚拟节点
        for(iter_type iter=hash_map.begin();iter!=hash_map.end();)
        {
            if(iter->second->get_virtual_id()==virtual_id)
            {
                hash_map.erase(iter++);
            }
            else
            {
                iter++;
            }
        }

        return true;

    }

    bool del_vnode_of_real_id(id_type real_id)
    {
        //删除指定real_id的虚拟节点
        for(iter_type iter=hash_map.begin();iter!=hash_map.end();)
        {
            if(iter->second->get_real_id()==real_id)
            {
                hash_map.erase(iter++);
            }
            else
            {
                iter++;
            }
        }

        return true;
    }


    bool find_real_id_of_key(int hash_key,id_type* real_id)
    {
        //在hash空间闭环里顺时针寻找virtual node,继而找到对应的真实节点id
        if (hash_map.size()==0)
            return false;

        iter_type iter=hash_map.lower_bound(hash_key);
        if(iter==hash_map.end())
        {
            iter=hash_map.begin();
        }
        *real_id=iter->second->get_real_id();
        return true;
    }

#ifdef DEBUG
    std::map<int,std::shared_ptr<Vnode<id_type>>>hash_map;
#endif



private:

#ifndef DEBUG
    std::map<int,std::shared_ptr<Vnode<id_type>>>hash_map;
#endif

    outer_func_type hash_func;

};

#endif //CONSISTENT_HASHING_CONSISTENT_HASHING_HPP
