#include "hashtbl.hpp"
#include <tuple>


struct Account
{
    using AcctKey = std::pair< std::string, int >;

    std::string name;
    int bank;
    int agency;
    int account;
    float balance;

    Account( std::string name_ = "default"
             , int bank_ = 0
             , int agency_ = 0
             , int account_ = 0
             , float balance_ = 0
             )
        : name( name_ )
        , bank( bank_ )
        , agency( agency_ )
        , account( account_ )
        , balance( balance_ )
    { /* empty */ }

    friend std::ostream & operator<<( std::ostream &os_, const Account &acc )
    {
        std::cout << "< " << acc.name << ". BANK(" << acc.bank << ") AG(" 
                  << acc.agency << ")" << " ACC:" << acc.account << ". R$ " << acc.balance << " >";
        return os_;
    }

    AcctKey curr_key( void )
    {
        return std::make_pair( name, account );
    }
};

struct KeyHash
{
    size_t operator()( const Account::AcctKey &key_ ) const
    {
        return std::hash< std::string >()( std::get<0>( key_ ) ) xor 
               std::hash< int >()( std::get<1>( key_ ) );
    }
};

struct KeyEqual
{
    bool operator()( const Account::AcctKey &lhs_, const Account::AcctKey &rhs_ ) const
    {
        return std::get<0>( lhs_ ) == std::get<0>( rhs_ ) 
           and std::get<1>( lhs_ ) == std::get<1>( rhs_ );
    }
};

int main()
{
    Account acc[] = { 
                      { "Alex Bastos", 1, 1668, 54321, 1500 },
                      { "Aline Souza", 1, 1668, 45794, 530 },
                      { "Cristiano Ronaldo", 13, 557, 87629, 150000 },
                      { "Jose Lima", 18, 331, 1231, 850 },
                      { "Saulo Cunha", 116, 666, 84312, 5490 }
                    };
    
    Hashtbl< Account::AcctKey, Account, KeyHash, KeyEqual > hash;

    // insert into the table
    std::cout << "Inserting 5 elements...\n";
    for( auto i(0); i != 5; ++i )
    {
        hash.insert( acc[i].curr_key(), acc[i] );
    }
    std::cout << "Table after inserting:\n";
    hash.print();
    std::cout << "\n";

    // removing an information
    auto temp_key = std::make_pair( "Alex Bastos", 54321 );
    std::cout << "Removing information of key < Alex Bastos, 54321 >... \n";
    hash.remove( temp_key );
    std::cout << "Table after removing:\n";
    hash.print();
    std::cout << "\n";

    // retrieving an information
    Account temp_acc;
    auto temp_key2 = std::make_pair( "Jose Lima", 1231 );
    std::cout << "Retrieving information of key < Jose Lima, 1231 >: \n";
    hash.retrieve( temp_key2, temp_acc );
    std::cout << temp_acc << "\n\n";

    std::cout << "Table count before crear: " << hash.count() << "\n";
    hash.clear();
    std::cout << "Table count after crear: " << hash.count() << "\n";

    std::cout << "Table at end:\n";
    hash.print();

    return EXIT_SUCCESS;
}