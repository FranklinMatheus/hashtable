/*!
 * @brief Hashtbl class methods implementations.
 * Author: Franklin Matheus.
 * Date:   11/06/2017
 */

// Interface

/*!
 * @brief Inserts into the table an information defined by data_ 
          that is associated with a key (key_). The function must 
          compute the address adrs which data_ will occupy and 
          store it in the list corresponding to the address (m_data_table[adrs]). 
          If the insertion succeeds, the function returns true. 
          Otherwise, if the key is already in the table, the function 
          overrides the table values corresponding to the key with those 
          contained in data_ and returns false.
 *
 * @param key_  key corresponding with the data.
 * @param data_ data to be inserted.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
bool Hashtbl< Key, Data, KeyHash, KeyEqual >::insert( const Key &key_, const Data &data_ )
{
    if( ( m_count/m_size ) > 1.0 ) rehash();

    KeyHash key_hash;
    KeyEqual key_equal;
    Entry new_entry( key_, data_ );
    auto adrs( key_hash( key_ ) % m_size );
    
    for( auto it( m_data_table[adrs].begin() ); it != m_data_table[adrs].end(); ++it )
    {
        if( key_equal( it->m_key, new_entry.m_key ) )
        {
            *it = new_entry;
            return false;
        }
    }
    m_data_table[adrs].push_back( new_entry );
    m_count++;

    return true;
}

/*!
 * @brief Removes an item represented by the key (key_) of the table. 
          If the key is found, it returns true, otherwise false.
 *
 * @param key corresponding to item to be removed.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
bool Hashtbl< Key, Data, KeyHash, KeyEqual >::remove( const Key &key_ )
{
    KeyHash key_hash;
    KeyEqual key_equal;

    auto adrs( key_hash( key_ ) % m_size );
    
    for( auto it( m_data_table[adrs].begin() ); it != m_data_table[adrs].end(); ++it )
    {
        if( key_equal( it->m_key, key_ ) )
        {
            m_data_table[adrs].erase( it );
            m_count--;
            return true;
        }
    }
    return false;
}

/*!
 * @brief Retrieves on data_ the information associated with the key (key_). 
          If the key is found, it returns true, otherwise false.
 *
 * @param key_  key of item to be retrieved.
 * @param data_ where the information will be retrieved.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
bool Hashtbl< Key, Data, KeyHash, KeyEqual >::retrieve( const Key &key_, Data &data_ ) const
{
    KeyHash key_hash;
    KeyEqual key_equal;

    auto adrs( key_hash( key_ ) % m_size );

    for( auto it( m_data_table[adrs].begin() ); it != m_data_table[adrs].end(); ++it )
    {
        if( key_equal( it->m_key, key_ ) )
        {
            data_ = it->m_data;
            return true;
        }
    }
    return false;
}

/*!
 * @brief Frees the memory associated with the lists, thus removing all the elements.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
void Hashtbl< Key, Data, KeyHash, KeyEqual >::clear( void )
{
    for( auto i(0u); i != m_size; ++i )
    {
        m_data_table[i].clear();    
    }
    m_count = 0;
}

/*!
 * @brief If the table is empty, it returns true, otherwise false.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
bool Hashtbl< Key, Data, KeyHash, KeyEqual >::empty( void )
{
    return m_count == 0;
}

/*!
 * @brief Returns the quantity of elements stored in the table.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
size_t Hashtbl< Key, Data, KeyHash, KeyEqual >::count( void ) const
{
    return m_count;
}

/*!
 * @brief Just a textual representation of the table and his elements.
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
void Hashtbl< Key, Data, KeyHash, KeyEqual >::print( void ) const
{
    for( auto i(0u); i != m_size; ++i )
    {
        std::cout << "[" << i << "] ----> ";
        for( auto &element : m_data_table[i] )
        {
            std::cout << element.m_data << " | ";
        }
        std::cout << "\n";
    }
}

/*!
 * @brief A function that if the number is prime, it returns true, otherwise false.
 * @param number the number to be tested.
 */
bool is_prime( size_t number )
{
    if( number <= 1 ) return false;
    if( number == 2 ) return true;

    for( auto i(3u); i != number; ++i )
    {
        if( number % i == 0 )
            return false;
    }

    return true;
}

/*!
 * @brief Its a private method that will be called when 
          the load factor (λ) gets higher than 1.0. 
          The load factor is the reason between the elements 
          number of table and his size. This method will resize
          the table according the following steps:
            1 - A new table will be created which size is equal to 
                the lowest prime number greater than twice the size 
              of table before calling rehash();
            2 - All the elements will be copied to the new table using a
                secondary dispersion function. 
 */
template < class Key, class Data, class KeyHash, class KeyEqual >
void Hashtbl< Key, Data, KeyHash, KeyEqual >::rehash( void )
{
    auto new_size( m_size*2 );
    while( not is_prime( new_size ) ) new_size++;
    
    Hashtbl< Key, Data, KeyHash, KeyEqual > new_table( new_size );

    for( auto i(0u); i != m_size; ++i )
    {
        for( auto it( m_data_table[i].begin() ); it != m_data_table[i].end(); ++it )
        {
            new_table.insert( it->m_key, it->m_data );
        }
    }
    m_size = new_size;
    m_count = new_table.count();
    m_data_table = new_table.m_data_table;
}