int foo( const int uiValue )
{
    int uiTrueValue = uiValue;

    if ( uiTrueValue < uiMinValue )
    {
        uiTrueValue = uiMinValue;
    }
    else if ( uiTrueValue > uiMaxValue )
    {
        uiTrueValue = uiMaxValue;
    }
    
    return uiTrueValue; // use...
}
