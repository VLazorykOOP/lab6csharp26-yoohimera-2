using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace StorageSystem
{
    // 1. Інтерфейс Товар, який успадковує стандартний інтерфейс .NET (IComparable)
    public interface ICommodity : IComparable<ICommodity>
    {
        string Name { get; }
        decimal Price { get; }
        
        // Метод для виведення інформації
        void PrintInfo();
        
        // Метод для перевірки на простроченість
        bool IsExpired(DateTime currentDate);
    }

    // 2. Похідний клас: Продукт
    public class Product : ICommodity
    {
        public string Name { get; set; }
        public decimal Price { get; set; }
        public DateTime ManufactureDate { get; set; }
        public int ExpirationDays { get; set; } // Строк придатності у днях

        public Product(string name, decimal price, DateTime manufactureDate, int expirationDays)
        {
            Name = name;
            Price = price;
            ManufactureDate = manufactureDate;
            ExpirationDays = expirationDays;
        }

        public void PrintInfo()
        {
            Console.WriteLine($"[Продукт] Назва: {Name,-15} | Ціна: {Price,6} грн | Дата вир.: {ManufactureDate:dd.MM.yyyy} | Придатний: {ExpirationDays} дн.");
        }

        public bool IsExpired(DateTime currentDate)
        {
            // Продукт прострочений, якщо дата виробництва + строк придатності менша за поточну дату
            return ManufactureDate.AddDays(ExpirationDays) < currentDate;
        }

        // Реалізація інтерфейсу IComparable для сортування за ціною
        public int CompareTo(ICommodity other)
        {
            if (other == null) return 1;
            return this.Price.CompareTo(other.Price);
        }
    }

    // 3. Похідний клас: Партія
    public class Batch : ICommodity
    {
        public string Name { get; set; }
        public decimal Price { get; set; }
        public int Quantity { get; set; }
        public DateTime ManufactureDate { get; set; }
        public int ExpirationDays { get; set; }

        public Batch(string name, decimal price, int quantity, DateTime manufactureDate, int expirationDays)
        {
            Name = name;
            Price = price;
            Quantity = quantity;
            ManufactureDate = manufactureDate;
            ExpirationDays = expirationDays;
        }

        public void PrintInfo()
        {
            Console.WriteLine($"[Партія]  Назва: {Name,-15} | Ціна: {Price,6} грн | К-ть: {Quantity,4} шт | Дата вир.: {ManufactureDate:dd.MM.yyyy} | Придатний: {ExpirationDays} дн.");
        }

        public bool IsExpired(DateTime currentDate)
        {
            return ManufactureDate.AddDays(ExpirationDays) < currentDate;
        }

        public int CompareTo(ICommodity other)
        {
            if (other == null) return 1;
            return this.Price.CompareTo(other.Price);
        }
    }

    // 4. Похідний клас: Комплект
    public class Kit : ICommodity
    {
        public string Name { get; set; }
        public decimal Price { get; set; }
        public List<Product> Products { get; set; }

        public Kit(string name, decimal price, List<Product> products)
        {
            Name = name;
            Price = price;
            Products = products;
        }

        public void PrintInfo()
        {
            Console.WriteLine($"[Комплект] Назва: {Name,-14} | Ціна: {Price,6} грн | Складається з:");
            foreach (var p in Products)
            {
                DateTime expDate = p.ManufactureDate.AddDays(p.ExpirationDays);
                Console.WriteLine($"   -> {p.Name} (Придатний до: {expDate:dd.MM.yyyy})");
            }
        }

        public bool IsExpired(DateTime currentDate)
        {
            // Комплект вважається простроченим, якщо хоча б один продукт у ньому прострочений
            return Products.Any(p => p.IsExpired(currentDate));
        }

        public int CompareTo(ICommodity other)
        {
            if (other == null) return 1;
            return this.Price.CompareTo(other.Price);
        }
    }

    // Головний клас програми
    class Program
    {
        static void Main(string[] args)
        {
            // Налаштування кодування для коректного відображення української мови в консолі
            Console.OutputEncoding = Encoding.UTF8;

            DateTime currentDate = DateTime.Now; // Поточна дата
            Console.WriteLine($"Поточна дата для перевірки: {currentDate:dd.MM.yyyy}\n");

            // Створення бази (масиву) товарів
            ICommodity[] database = new ICommodity[]
            {
                // Продукти (один свіжий, один прострочений для перевірки)
                new Product("Молоко", 45.50m, DateTime.Now.AddDays(-10), 14),     // Ще придатне
                new Product("Кефір", 48.00m, DateTime.Now.AddDays(-20), 14),      // Прострочений

                // Партії
                new Batch("Яблука", 25.00m, 100, DateTime.Now.AddDays(-5), 30),   // Свіжа партія
                new Batch("Банани", 60.00m, 50, DateTime.Now.AddDays(-15), 10),   // Прострочена партія

                // Комплекти
                new Kit("Сніданок", 150.00m, new List<Product>
                {
                    new Product("Вівсянка", 30.00m, DateTime.Now.AddMonths(-1), 180),
                    new Product("Йогурт", 25.00m, DateTime.Now.AddDays(-5), 10)
                }), // Свіжий комплект

                new Kit("Подарунковий", 300.00m, new List<Product>
                {
                    new Product("Шоколад", 50.00m, DateTime.Now.AddMonths(-1), 180),
                    new Product("Цукерки", 100.00m, DateTime.Now.AddDays(-40), 30) // Цукерки прострочені
                }) // Прострочений комплект
            };

            // 1. Виведення повної інформації з бази на екран
            Console.WriteLine("=== ПОВНА ІНФОРМАЦІЯ ПРО ТОВАРИ В БАЗІ ===");
            foreach (var item in database)
            {
                item.PrintInfo();
                Console.WriteLine(new string('-', 80));
            }

            // 2. Пошук простроченого товару на момент поточної дати
            Console.WriteLine("\n=== СПИСОК ПРОСТРОЧЕНИХ ТОВАРІВ ===");
            bool foundExpired = false;

            foreach (var item in database)
            {
                if (item.IsExpired(currentDate))
                {
                    foundExpired = true;
                    Console.ForegroundColor = ConsoleColor.Red; // Виділяємо червоним кольором
                    item.PrintInfo();
                    Console.ResetColor();
                }
            }

            if (!foundExpired)
            {
                Console.WriteLine("Прострочених товарів не знайдено. Все свіже!");
            }

            Console.ReadLine();
        }
    }
}
