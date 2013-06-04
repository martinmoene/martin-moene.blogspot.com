void foo()
{
    std11::exception_ptr p =
       std11::make_exception_ptr( std::invalid_argument("invalid argument") );

    std11::exception_ptr q = p;

    std11::rethrow_exception( p );
}
