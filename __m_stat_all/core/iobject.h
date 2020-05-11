#ifndef IOBJECT_H
#define IOBJECT_H

namespace pg {
    class Answer;
}

/// Базовый класс для представления объектов базы данных
class IObject
{

public:

    IObject();
    virtual ~IObject() noexcept;

    /// Функция преобразования ответа базы данных в пользовательский объект
    virtual IObject * fromPgAnswer( pg::Answer * answer, unsigned long i ) = 0;
};

#endif // IOBJECT_H
