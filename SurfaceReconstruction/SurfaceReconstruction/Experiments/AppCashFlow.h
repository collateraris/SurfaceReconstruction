#pragma once

#include "SafePtr.h"

#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <utility>

namespace Experiments::Threads 
{
    class AppCashFlow
    {
        struct user_accounts_t 
        { 
            std::string user_name; 
            int64_t money; 
            user_accounts_t(std::string u, int64_t m) 
                : user_name(u), money(m) {}
        };

        SafePtr<std::map<uint64_t, user_accounts_t >> safe_user_accounts;

        struct cash_flows_t 
        { 
            uint64_t unique_id, src_id, dst_id, time; 
            int64_t money; 
        };

        std::atomic<uint64_t> global_unique_id;
        SafePtr<std::multimap<uint64_t, std::shared_ptr<cash_flows_t>>> safe_cash_flows_src_id;
        SafePtr<std::multimap<uint64_t, std::shared_ptr<cash_flows_t>>> safe_cash_flows_dst_id;
    
        void move_money(uint64_t src_id, uint64_t dst_id, uint64_t time, int64_t money)
        {
            auto cash_flow_row_ptr = std::make_shared<cash_flows_t>();
            cash_flow_row_ptr->unique_id = ++global_unique_id;
            cash_flow_row_ptr->src_id = src_id;
            cash_flow_row_ptr->dst_id = dst_id;
            cash_flow_row_ptr->time = time;
            cash_flow_row_ptr->money = money;

            std::cout << " - start transaction... move_money() \n";

            std::lock_guard<decltype(safe_user_accounts)> lock1(safe_user_accounts);
            std::lock_guard<decltype(safe_cash_flows_src_id)> lock2(safe_cash_flows_src_id);
            std::lock_guard<decltype(safe_cash_flows_dst_id)> lock3(safe_cash_flows_dst_id);

            // update table-1
            safe_user_accounts->at(src_id).money -= money;            
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // for example - OS decides to interrupt this thread
            safe_user_accounts->at(dst_id).money += money;

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // for example - OS decides to interrupt this thread

            // inset to indexes in table safe_cash_flows: src & dst
            safe_cash_flows_src_id->emplace(src_id, cash_flow_row_ptr);
            safe_cash_flows_dst_id->emplace(dst_id, cash_flow_row_ptr);
        
            std::cout << " - end transaction: move_money() \n";
        }

        void show_total_amount() 
        {
                int64_t total_amount = 0;

                std::cout << " - start transaction... show_total_amount() \n";

                std::lock_guard<decltype(safe_user_accounts)> lock(safe_user_accounts);
                    
                std::cout << std::endl;
                for (auto it = safe_user_accounts->begin(); it != safe_user_accounts->end(); ++it) {
                    total_amount += it->second.money;
                    std::cout << it->first << " => " << it->second.user_name << ", " <<
                        it->second.money << std::endl;
                }
                std::cout << "Result: all accounts total_amount = " << total_amount << " \t <<< \n\n";

                std::cout << " - end transaction: show_total_amount() \n";
        }  

         void show_user_money_on_time(uint64_t user_id, uint64_t time)
        {
                int64_t incoming = 0;
                int64_t outcoming = 0;
                int64_t user_money = 0;
                std::string user_name;

                std::cout << " - start transaction... show_user_money_on_time() \n";

                std::lock_guard<decltype(safe_user_accounts)> lock1(safe_user_accounts);
                std::lock_guard<decltype(safe_cash_flows_src_id)> lock2(safe_cash_flows_src_id);
                std::lock_guard<decltype(safe_cash_flows_dst_id)> lock3(safe_cash_flows_dst_id);

                std::cout << std::endl;
                auto in_range = safe_cash_flows_dst_id->equal_range(user_id);
                for (auto it = in_range.first; it != in_range.second; ++it)
                    if (it->second->time > time)
                        incoming += it->second->money;

                auto out_range = safe_cash_flows_src_id->equal_range(user_id);
                for (auto it = out_range.first; it != out_range.second; ++it)
                    if (it->second->time > time)
                        outcoming += it->second->money;

                user_money = safe_user_accounts->at(user_id).money;
                user_name = safe_user_accounts->at(user_id).user_name;

                std::cout << std::endl << "incoming = " << incoming << ", outcoming = " << outcoming <<
                    ", current user_money = " << user_money << std::endl;

                user_money = user_money - incoming + outcoming; // take into account cash flow

                std::cout << user_id << " => " << user_name << ", " << user_money <<
                    ", at time = " << time << " \t <<< \n\n";

                std::cout << " - end transaction: show_user_money_on_time() \n";
        } 

     public:
        void testRun()
        {
            std::cout << "Init table safe_user_accounts: " << std::endl;
            std::cout << "at time = 0  \t\t <<< " << std::endl;

            SafePtr<std::map<uint64_t, user_accounts_t >> _safe_user_accounts(
                std::map<uint64_t, user_accounts_t >({
                std::make_pair(1, user_accounts_t("John Smith", 100)),
                std::make_pair(2, user_accounts_t("John Rambo", 150))
                    }));

            safe_user_accounts = std::move(_safe_user_accounts);

            for (auto it = safe_user_accounts->begin(); it != safe_user_accounts->end(); ++it)
                std::cout << it->first << " => " << it->second.user_name << ", " <<
                it->second.money << std::endl;
                std::cout << std::endl;


            std::thread t1([&]() { move_money(2, 1, 12, 50); });    // src_id, dst_id, time, money

            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // OS decides that another thread runs at this time   

            std::thread t2([&]() { show_total_amount(); });

            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // OS decides that another thread runs at this time   

            std::thread t3([&]() { show_user_money_on_time(1, 0); });   // user_id, time

            t1.join();
            t2.join();
            t3.join();
        }      
    };
}