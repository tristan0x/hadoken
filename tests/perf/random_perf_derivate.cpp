/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 * 
 * Boost Software License - Version 1.0 
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
* 
*/




#include <boost/test/floating_point_comparison.hpp>


#include <boost/random.hpp>
#include <boost/chrono.hpp>

#include <hadoken/random/random.hpp>


using namespace boost::chrono;

typedef  system_clock::time_point tp;
typedef  system_clock cl;


std::size_t test_random_mersenne_twister(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::mt19937 twister_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        boost::random::mt19937 derivated_engine = hadoken::random_engine_derivate(twister_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "mersenne_twister: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}


std::size_t test_random_abstract_mersenne_twister(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::mt19937 twister_engine;
    hadoken::random_engine_mapper_32 abstract_engine(twister_engine);

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
    hadoken::random_engine_mapper_32 derivated_engine = hadoken::random_engine_derivate(abstract_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "abstract_mersenne_twister: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}




std::size_t test_random_ranlux(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::ranlux24 ranlux_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        boost::random::ranlux24 derivated_engine = hadoken::random_engine_derivate(ranlux_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "ranlux: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}




std::size_t test_random_abstract_ranlux(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::ranlux24 ranlux_engine;
    hadoken::random_engine_mapper_32 abstract_engine(ranlux_engine);

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        hadoken::random_engine_mapper_32 derivated_engine = hadoken::random_engine_derivate(abstract_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "abstract_ranlux: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}


std::size_t test_random_tau88(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::taus88 tau_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        boost::random::taus88 derivated_engine = hadoken::random_engine_derivate(tau_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "tau88: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}




std::size_t test_random_abstract_tau88(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    boost::random::taus88 tau_engine;
    hadoken::random_engine_mapper_32 abstract_engine(tau_engine);

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        hadoken::random_engine_mapper_32 derivated_engine = hadoken::random_engine_derivate(abstract_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "abstract_tau88: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}



std::size_t test_random_abstract_threefry(std::size_t iter){

    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;
    hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;
    hadoken::random_engine_mapper_64 abstract_engine(threefry_engine);

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        hadoken::random_engine_mapper_64  derivated_engine = hadoken::random_engine_derivate(abstract_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "abstract_threefry: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}


std::size_t test_random_threefry(std::size_t iter){

    std::size_t res =0;

    tp t1, t2;

    boost::random::uniform_int_distribution<std::size_t> dist;

    hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        hadoken::counter_engine<hadoken::threefry4x64> derivated_engine = hadoken::random_engine_derivate(threefry_engine, 1);
        res += dist(derivated_engine);
    }


    t2 = cl::now();

    std::cout << "threefry: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}


int main(){

    const std::size_t n_exec = 1000000;
    std::size_t junk=0;

    std::cout << "generation of " << n_exec << " derivate random generators with one generator" << std::endl;

    junk += test_random_mersenne_twister(n_exec);


    junk += test_random_abstract_mersenne_twister(n_exec);


    junk += test_random_ranlux(n_exec);


    junk += test_random_abstract_ranlux(n_exec);


    junk += test_random_tau88(n_exec);


    junk += test_random_abstract_tau88(n_exec);


    junk += test_random_threefry(n_exec);


    junk += test_random_abstract_threefry(n_exec);

    std::cout << "end junk " << junk << std::endl;

}
