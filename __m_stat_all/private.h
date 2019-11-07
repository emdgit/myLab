#ifndef PRIVATE_H
#define PRIVATE_H

#define DECLARE_PRIVATE(Class) \
    inline Class##Private* m_func() \
    { return reinterpret_cast<Class##Private *>(qGetPtrHelper(m_ptr)); } \
    inline const Class##Private* m_func() const \
    { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(m_ptr)); } \
    friend class Class##Private;

#define QQ(Class) Class##Private * const m = m_func()

#endif // PRIVATE_H
