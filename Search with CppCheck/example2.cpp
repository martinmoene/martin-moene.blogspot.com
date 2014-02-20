int foo( const int value )
{
    int clampedValue = value;
    
    if      ( clampedValue < m_SomeMin ) clampedValue = m_SomeMin;
    else if ( clampedValue > m_SomeMax ) clampedValue = m_SomeMax;

    return clampedValue; // use...
}
