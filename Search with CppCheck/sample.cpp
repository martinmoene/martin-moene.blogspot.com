// matching:

int if_if( int value )
{
    if ( value < minValue )
    {
        value = minValue;
    }
    if ( value > minValue )
    {
        value = minValue;
    }

    return value; // use...
}

int if_else_if( const int value )
{
    int uiTrueValue = uiValue;

    if ( uiTrueValue < uiMinValue )
    {
        uiTrueValue = uiMinValue;
    }
    else if ( value > uiMaxValue )
    {
        uiTrueValue = uiMaxValue;
    }

    return uiTrueValue; // use...
}

int if_if_compact( const int value )
{
    int clampedValue = value;

    if ( clampedValue < minValue ) clampedValue = minValue;
    if ( clampedValue > maxValue ) clampedValue = maxValue;

    return clampedValue; // use...
}

int if_else_if_compact( const int value )
{
    int clampedValue = value;

    if      ( clampedValue < minValue ) clampedValue = minValue;
    else if ( clampedValue > maxValue ) clampedValue = maxValue;

    return clampedValue; // use...
}

int if_else_if_less_equal_compact( const int value )
{
    int clampedValue = value;

    if      ( clampedValue <= minValue ) clampedValue = minValue;
    else if ( clampedValue >= maxValue ) clampedValue = maxValue;

    return clampedValue; // use...
}

int if_else_if_numeric_compact( const int value )
{
    int clampedValue = value;

    if      ( clampedValue < 123 ) clampedValue = 123;
    else if ( clampedValue > 987 ) clampedValue = 987;

    return clampedValue; // use...
}

// desired not to match (longer if chain)

int if_else_if_else_if_numeric_compact( const int value )
{
    int clampedValue = value;

    if      ( clampedValue < 123 ) clampedValue = 123;
    else if ( clampedValue > 987 ) clampedValue = 987;
    else if ( clampedValue > 321 ) clampedValue = 321;

    return clampedValue; // use...
}

// non-matching:

int nomatch_if_once( int value )
{
    if ( value < minValue )
    {
        value = minValue;
    }

    return value; // use...
}

int nomatch_equal( int value )
{
    if      ( value <= minValue ) value = minValue;
    else if ( value == maxValue ) value = maxValue;

    return value; // use...
}

// trigger CppCheck core style issue:

void style_not_used()
{
   int arg;
}

// trigger CppCheck core error:

char * error_return_address_of_local_var()
{
    char text = 'a';
    return &text;
}

// trigger CppCheck core scope style warning

void foo()
{
    int var;

    if ( true )
    {
        var = 7;
    }
}
