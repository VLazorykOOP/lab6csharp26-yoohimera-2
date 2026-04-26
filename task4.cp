using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace EnumeratorExample
{
    // Клас Продукт (спрощений для демонстрації)
    public class Product
    {
        public string Name { get; set; }
        public decimal Price { get; set; }

        public Product(string name, decimal price)
        {
            Name = name;
            Price = price;
        }

        public override string ToString()
        {
            return $"{Name,-15} | {Price,6} грн";
        }
    }

    // Клас Комплект, який реалізує інтерфейс IEnumerable<Product>
    public class Kit : IEnumerable<Product>
    {
        public string Name { get; set; }
        
        // Робимо список приватним (інкапсуляція). Доступ до нього ззовні тепер 
        // буде здійснюватися через ітератор (foreach).
        private List<Product> _products = new List<Product>();

        public Kit(string name)
        {
            Name = name;
        }

        public void AddProduct(Product product)
        {
            _products.Add(product);
        }

        // ====================================================================
        // РЕАЛІЗАЦІЯ ІНТЕРФЕЙСІВ .NET ДЛЯ ПІДТРИМКИ foreach
        // ====================================================================

        // 1. Типізований перелічувач (повертає IEnumerator<Product>)
        public IEnumerator<Product> GetEnumerator()
        {
            // Використовуємо вбудований перелічувач списку
            return _products.GetEnumerator();
            
            // Альтернативний варіант за допомогою yield:
            // foreach (var item in _products)
            // {
            //     yield return item;
            // }
        }

        // 2. Нетипізований перелічувач (необхідний для зворотної сумісності .NET)
        IEnumerator IEnumerable.GetEnumerator()
        {
            // Просто викликаємо типізований метод, реалізований вище
            return GetEnumerator();
        }
    }

    // Головний клас програми
    class Program
    {
        static void Main()
        {
            Console.OutputEncoding = Encoding.UTF8;

            // Створюємо комплект
            Kit giftKit = new Kit("Подарунковий кошик");

            // Додаємо продукти
            giftKit.AddProduct(new Product("Шоколад", 85.50m));
            giftKit.AddProduct(new Product("Кава", 250.00m));
            giftKit.AddProduct(new Product("Чай", 120.00m));

            Console.WriteLine($"=== Склад комплекту: '{giftKit.Name}' ===\n");

            // Завдяки інтерфейсу IEnumerable ми тепер можемо використовувати 
            // оператор foreach БЕЗПОСЕРЕДНЬО до об'єкта класу Kit
            foreach (Product item in giftKit)
            {
                Console.WriteLine($"-> {item}");
            }

            Console.ReadLine();
        }
    }
}
