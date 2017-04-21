template <typename T>
class FixedArray
{
  T* m_array;
  size_t m_size;
public:
  FixedArray(){}
  FixedArray(size_t size)
    : m_array(0), m_size(0)
  {
    resize(size);
  }

  ~FixedArray()
  {
    free ();
  }

  void resize(size_t size)
  {
    if (size <= 0)
      return;

    if (m_array)
      {
	if (size > m_size)
	  {
	    T* tmp = new T[size];
	    for (int i = 0; i < m_size; ++i)
	      tmp[i] = m_array[i];
	    delete[] m_array;
	    m_array = tmp;
	    m_size = size;
	  }
	else
	  {
	    // enough, don't do anything
	    // en, you can re-allocate it
	  }
      }
    else
      {
	m_array = new T[size];
	m_size = size;
      }
  }
  void free ()
  {
    if (m_array)
      {
	delete[] m_array;
	m_array = 0;
	m_size = 0;
      }
  }
  T& operator[](int i)
  {
    // if (!m_array || (i < 0) || (i >= m_size))
    //   throw "out of range";
    return m_array[i];
  }

  const T& operator[] (int i) const
  {
    // if (!m_array || (i < 0) || (i >= m_size))
    //   throw "out of range";
    return m_array[i];
  }
};
