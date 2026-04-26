using System;

namespace InterfaceHierarchy
{
    // ==========================================
    // 1. Користувацькі інтерфейси
    // ==========================================
    public interface IPersonInfo
    {
        string Name { get; set; }
        int Age { get; set; }
        void Show(); // Інтерфейсний метод
    }

    public interface IUniversityMember
    {
        void PerformDuties(); // Інтерфейсний метод
    }

    // ==========================================
    // 2. Реалізація класів
    // ==========================================

    // Клас Person реалізує користувацький інтерфейс та інтерфейс .NET (IComparable)
    public class Person : IPersonInfo, IComparable<Person>
    {
        public string Name { get; set; }
        public int Age { get; set; }

        public Person(string name, int age)
        {
            Name = name;
            Age = age;
        }

        public virtual void Show()
        {
            Console.WriteLine($"[Персона] Ім'я: {Name, -15} | Вік: {Age}");
        }

        // Реалізація інтерфейсу .NET
        public int CompareTo(Person other)
        {
            if (other == null) return 1;
            return this.Age.CompareTo(other.Age);
        }

        // Особистий (унікальний) метод класу Person
        public void Relax()
        {
            Console.WriteLine($"   *Особистий метод*: {Name} просто відпочиває.");
        }
    }

    // Клас Student додатково реалізує інтерфейс IUniversityMember
    public class Student : Person, IUniversityMember
    {
        public int Course { get; set; }

        public Student(string name, int age, int course) : base(name, age)
        {
            Course = course;
        }

        public override void Show()
        {
            Console.WriteLine($"[Студент] Ім'я: {Name, -15} | Вік: {Age} | Курс: {Course}");
        }

        public void PerformDuties()
        {
            Console.WriteLine($"   -> Обов'язок: {Name} відвідує пари та робить ДЗ.");
        }

        // Особистий метод
        public void TakeExam()
        {
            Console.WriteLine($"   *Особистий метод*: {Name} готується до сесії.");
        }
    }

    // Клас Teacher
    public class Teacher : Person, IUniversityMember
    {
        public string Subject { get; set; }

        public Teacher(string name, int age, string subject) : base(name, age)
        {
            Subject = subject;
        }

        public override void Show()
        {
            Console.WriteLine($"[Викладач] Ім'я: {Name, -14} | Вік: {Age} | Дисципліна: {Subject}");
        }

        public virtual void PerformDuties()
        {
            Console.WriteLine($"   -> Обов'язок: {Name} проводить лекції з предмету {Subject}.");
        }

        // Особистий метод
        public void GradeExams()
        {
            Console.WriteLine($"   *Особистий метод*: {Name} перевіряє екзаменаційні роботи.");
        }
    }

    // Клас DepartmentHead
    public class DepartmentHead : Teacher
    {
        public string DepartmentName { get; set; }

        public DepartmentHead(string name, int age, string subject, string departmentName) 
            : base(name, age, subject)
        {
            DepartmentName = departmentName;
        }

        public override void Show()
        {
            Console.WriteLine($"[Зав. каф.] Ім'я: {Name, -13} | Вік: {Age} | Кафедра: {DepartmentName}");
        }

        public override void PerformDuties()
        {
            Console.WriteLine($"   -> Обов'язок: {Name} керує роботою кафедри '{DepartmentName}'.");
        }

        // Особистий метод
        public void HoldDepartmentMeeting()
        {
            Console.WriteLine($"   *Особистий метод*: {Name} проводить засідання кафедри.");
        }
    }

    // ==========================================
    // 3. Головна програма
    // ==========================================
    class Program
    {
        // Метод для виклику унікальних методів за допомогою Type Pattern
        static void InvokeUniqueMethods(IPersonInfo[] peopleArray)
        {
            Console.WriteLine("\n=== ВИКЛИК ОСОБИСТИХ МЕТОДІВ (TYPE PATTERN) ===");
            foreach (var person in peopleArray)
            {
                // Використовуємо switch з паттерном типів (C# 7.0+)
                // ВАЖЛИВО: Порядок має значення! Від найбільш похідних (DepartmentHead) до базових (Person)
                switch (person)
                {
                    case DepartmentHead head:
                        head.HoldDepartmentMeeting();
                        break;
                    case Teacher teacher:
                        teacher.GradeExams();
                        break;
                    case Student student:
                        student.TakeExam();
                        break;
                    case Person p:
                        p.Relax();
                        break;
                    default:
                        Console.WriteLine("Невідомий тип.");
                        break;
                }
            }
        }

        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            // Створюємо масив інтерфейсних посилань (IPersonInfo)
            IPersonInfo[] universityPeople = new IPersonInfo[]
            {
                new Person("Григорій", 30),
                new Student("Олександр", 19, 2),
                new Teacher("Іван Петрович", 45, "Алгоритми"),
                new DepartmentHead("Микола Іванович", 55, "Архітектура ПК", "Кафедра ІТ")
            };

            // Створюємо другий масив інтерфейсних посилань (IUniversityMember)
            // Персона сюди не входить, бо вона не реалізує цей інтерфейс
            IUniversityMember[] staffAndStudents = new IUniversityMember[]
            {
                new Student("Марія", 20, 3),
                new Teacher("Олена Василівна", 38, "Бази даних"),
                new DepartmentHead("Анна Сергіївна", 50, "Кібербезпека", "Кафедра БІ")
            };

            // 1. Виклик інтерфейсних методів для першого масиву
            Console.WriteLine("=== ІНТЕРФЕЙСНІ МЕТОДИ (IPersonInfo.Show) ===");
            foreach (var person in universityPeople)
            {
                person.Show();
            }

            // 2. Виклик інтерфейсних методів для другого масиву
            Console.WriteLine("\n=== ІНТЕРФЕЙСНІ МЕТОДИ (IUniversityMember.PerformDuties) ===");
            foreach (var member in staffAndStudents)
            {
                member.PerformDuties();
            }

            // 3. Виклик особистих методів через Type Pattern
            InvokeUniqueMethods(universityPeople);

            // 4. Демонстрація інтерфейсу .NET (IComparable)
            Console.WriteLine("\n=== ДЕМОНСТРАЦІЯ .NET ІНТЕРФЕЙСУ (Сортування Person за віком) ===");
            Person[] justPeople = { new Person("Антон", 40), new Person("Борис", 25) };
            Array.Sort(justPeople); // Працює завдяки IComparable<Person>
            foreach(var p in justPeople) p.Show();

            Console.ReadLine();
        }
    }
}
