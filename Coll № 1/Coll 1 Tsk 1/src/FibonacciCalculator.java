import java.util.ArrayList;
import java.util.List;

public class FibonacciCalculator {

    public static List<Integer> calculateFibonacciNumbers(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("n должно быть положительным числом");
        }

        List<Integer> fibonacciNumbers = new ArrayList<>();

        int prev = 0;
        int curr = 1;

        fibonacciNumbers.add(prev);

        for (int i = 1; i < n; i++) {
            fibonacciNumbers.add(curr);
            int next = prev + curr;
            prev = curr;
            curr = next;
        }

        return fibonacciNumbers;
    }

    public static void main(String[] args) {
        try {
            List<Integer> fibonacciNumbers = calculateFibonacciNumbers(10);
            System.out.println("Первые 10 чисел Фибоначчи: " + fibonacciNumbers);
        } catch (IllegalArgumentException e) {
            System.err.println(e.getMessage());
        }
    }
}
