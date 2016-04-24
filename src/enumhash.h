#ifndef ENUMHASH_H
#define ENUMHASH_H

class EnumHash
{
public:
    template <typename T>
    size_t operator () (const T& x) const {
        return getResult<T>( x, std::is_enum<T>() );
    }

private:
    template <typename T>
    size_t getResult( const T& x, std::true_type ) const {
        using utype = typename std::underlying_type<T>::type;
        return std::hash<utype>()( static_cast<utype>( x ) );
    }
};


#endif // ENUMHASH_H
