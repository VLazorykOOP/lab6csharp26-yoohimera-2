using System;
using System.Text;

namespace ExceptionHandlingApp
{
    // 1. Створення власного класу винятку шляхом успадкування від Exception
    public class CustomMathException : Exception
    {
        // Конструктор за замовчуванням
        public CustomMathException() : base("Сталася специфічна математична помилка.") 
        { }

        // Конструктор з повідомленням
        public CustomMathException(string message) : base(message) 
        { }

        // Конструктор з повідомленням та внутрішнім винятком (InnerException)
        public CustomMathException(string message, Exception innerException) : base(message, innerException) 
        { }
    }

    class Program
    {
        // Метод, який виконує ділення
        static int DivideNumbers(int numerator, int denominator)
        {
            // Штучно генеруємо наш власний виняток для певних умов
            // Наприклад, заборонимо ділити число 13 (як "нещасливе" число)
            if (numerator == 13)
            {
                throw new CustomMathException("Операції з числом 13 штучно заборонені нашою бізнес-логікою!");
            }

            // Якщо denominator дорівнює 0, C# автоматично згенерує DivideByZeroException
            // під час ділення цілих чисел (int)
            return numerator / denominator;
        }

        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            Console.WriteLine("=== Програма для ділення двох чисел (Демонстрація обробки винятків) ===");

            while (true)
            {
                try
                {
                    Console.Write("\nВведіть ділене (або 'q' для виходу): ");
                    string input1 = Console.ReadLine();
                    if (input1.ToLower() == "q") break;

                    Console.Write("Введіть дільник: ");
                    string input2 = Console.ReadLine();

                    // Конвертація рядка в число може викликати FormatException
                    int num1 = int.Parse(input1);
                    int num2 = int.Parse(input2);

                    // Виклик методу, який може згенерувати винятки
                    int result = DivideNumbers(num1, num2);
                    
                    Console.WriteLine($"Результат: {num1} / {num2} = {result}");
                }
                // 2. Обробка стандартного винятку згідно з варіантом 3.2
                catch (DivideByZeroException ex)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine($"[ПОМИЛКА ДІЛЕННЯ НА НУЛЬ]: {ex.Message}");
                    Console.WriteLine("-> Підказка: Дільник не може дорівнювати нулю.");
                    Console.ResetColor();
                }
                // 3. Обробка іншого стандартного винятку (помилка формату вводу)
                catch (FormatException ex)
                {
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.WriteLine($"[ПОМИЛКА ВВОДУ]: {ex.Message}");
                    Console.WriteLine("-> Підказка: Вводьте лише цілі числа, літери не допускаються.");
                    Console.ResetColor();
                }
                // 4. Обробка нашого власного винятку
                catch (CustomMathException ex)
                {
                    Console.ForegroundColor = ConsoleColor.Magenta;
                    Console.WriteLine($"[ВЛАСНИЙ ВИНЯТОК]: {ex.Message}");
                    Console.ResetColor();
                }
                // 5. Загальний перехоплювач для всіх інших непередбачених помилок (має бути останнім)
                catch (Exception ex)
                {
                    Console.ForegroundColor = ConsoleColor.DarkRed;
                    Console.WriteLine($"[КРИТИЧНА ПОМИЛКА]: Сталася непередбачена ситуація: {ex.Message}");
                    Console.ResetColor();
                }
            }
            
            Console.WriteLine("Роботу програми завершено.");
        }
    }
}
