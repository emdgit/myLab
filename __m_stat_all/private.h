#ifndef PRIVATE_H
#define PRIVATE_H

#define DECLARE_PRIVATE(Class) \
    inline Class##Private* owl_func() \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(qGetPtrHelper(owl_ptr));) } \
    inline const Class##Private* owl_func() const \
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(qGetPtrHelper(owl_ptr));) } \
    friend class Class##Private;

#define QQ(Class) Class##Private * const d = owl_func()

#endif // PRIVATE_H
