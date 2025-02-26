#include <bits/stdc++.h>
#include "public_ledger.h"
#include "mempad.h"
#include "wallet.h"
#include "Transaction.h"

//this file will control the entire working and network handling is yet to be added too
using namespace std;

// ✅ Function to DELETE transactions.json after adding a block
void delete_transactions_file() {
    if (remove("transactions.json") == 0) {
        cout << "\n[🗑️] Deleted transactions.json after adding a block!\n";
    } else {
        cout << "\n[⚠️] Warning: transactions.json could not be deleted!\n";
    }
}

int main()
{
    // Create mempool instance to retrieve transactions
    // as soon as this instance is created the vector of json objects in which the json files of all the broadcasted transactions will be collected
    // these json object vector transaction_records will then be converted back to strings and stored in each block which will then be mined and added to the local copy of the public ledger class
    // why we are converting to json!?
    // to send the transactions to the peer-to-peer network that we will create in network.cpp via Golang and socket programming
    // this network.cpp has to contain the code for broadcasting the already json format transactions and the local copy of the public_ledger AFTER MINING THE BLOCK
    // whosoever mines first his public_ledger will be broadcasted and everyone will take that public ledger as their main public ledger mining for the further blocks
    // if possible we need to add in the script or code for the deletion of transactions.json after a block is added in the public_ledger

    // ✅ Step 1: Create a wallet for the sender
    Wallet w1;  // automatically creates an instance of private and a public key

    // ✅ Step 2: Create two test transactions
    string receiver1 = "8455fw5665651carca";  // enter the public addresses of the receivers from their wallets
    string receiver2 = "wlkfchbwoiurh4u904u";
    
    Transaction t1(w1.public_key, receiver1, 6);   // creating transactions
    t1.save_to_file();    // saving transactions to json files ..... these will be broadcasted over the network 

    Transaction t2(w1.public_key, receiver2, 10);
    t2.save_to_file();

    // ✅ Step 3: Initialize mempool & read transactions
    mempool m1;
    vector<string> all_trans_in_string_format = m1.convert_json_to_strings();

    // ✅ Step 4: Initialize Blockchain with Genesis Block (containing first 2 transactions)
    Blockchain miniBTC_chain;
    miniBTC_chain.add_block(all_trans_in_string_format); // Adding the first non-genesis block

    delete_transactions_file();  // ✅ DELETE transactions.json after block is added

    // ✅ Step 5: Display Blockchain Info
    miniBTC_chain.display_chain();

    // ----------------------------------------
    // ✅ Step 6: Adding another block to the blockchain
    // ----------------------------------------

    cout << "\n\nCreating additional transactions for the next block...\n";

    string receiver3 = "12345abcde09876xyz";  // New transactions for next block
    string receiver4 = "zxcvbnm567890poiuy";

    Transaction t3(w1.public_key, receiver3, 15);  // creating another transaction
    t3.save_to_file();

    Transaction t4(w1.public_key, receiver4, 20);
    t4.save_to_file();

    // ✅ Step 7: Read only newly added transactions
    mempool m2;  // Create a fresh mempool to read new transactions
    vector<string> new_transactions = m2.convert_json_to_strings();

    // ✅ Step 8: Add another block with new transactions
    miniBTC_chain.add_block(new_transactions);
    delete_transactions_file();  // ✅ DELETE transactions.json after block is added

    // ✅ Step 9: Display Updated Blockchain Info
    miniBTC_chain.display_chain();

    return 0;
}
