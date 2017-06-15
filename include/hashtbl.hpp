#ifndef _HASHTBL_HPP_
#define _HASHTBL_HPP_

    #include <list>         // <! list
    #include <cstddef>      // <! size_T
    #include <iostream>     // <! cout

    template < typename Key, typename Data >
    struct HashEntry
    {
        Key m_key;
        Data m_data;

        HashEntry( Key k_, Data d_ )
            : m_key( k_ )
            , m_data( d_ )
        { /* empty */ }

        void operator=( const HashEntry &b_ )
        {
            m_key = b_.m_key;
            m_data = b_.m_data;
        }
    };

    template < typename Key, typename Data, 
               typename KeyHash = std::hash< Key >, 
               typename KeyEqual = std::equal_to< Key > >
    class Hashtbl
    {
        public:
            // Alias
            using Entry = HashEntry< Key, Data >;
            using array_t = std::list< Entry >;

        private:
            void rehash( void );
            
            // Members
            size_t m_size;
            size_t m_count;
            array_t *m_data_table;
            
            // Default definition
            static const short DEFAULT_SIZE = 11;
        public:
            // Cons(des)tructor
            Hashtbl( int tbl_size = DEFAULT_SIZE )
                : m_size( tbl_size )
                , m_count( 0 )
                , m_data_table( new array_t[ tbl_size+1 ] )
            { /* empty */ }

            virtual ~Hashtbl() = default;

            // Interface
            bool insert( const Key &key_, const Data &data_ );
            bool remove( const Key &key_ );
            bool retrieve( const Key &key_, Data &data_ ) const;
            void clear( void );
            bool empty( void );
            size_t count( void ) const;
            void print( void ) const;
    };
    
    #include "hashtbl.inl"

#endif