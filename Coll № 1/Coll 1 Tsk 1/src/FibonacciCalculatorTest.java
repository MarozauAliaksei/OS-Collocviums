import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class FibonacciCalculatorTest {

    @Test
    public void testCalculateFibonacciNumbers() {
        List<Integer> fibonacciNumbers = FibonacciCalculator.calculateFibonacciNumbers(5);
        assertEquals(List.of(0, 1, 1, 2, 3), fibonacciNumbers);
    }

    @Test
    public void testCalculateFibonacciNumbersWithZero() {
        assertThrows(IllegalArgumentException.class, () -> FibonacciCalculator.calculateFibonacciNumbers(0));
    }

    @Test
    public void testCalculateFibonacciNumbersWithNegative() {
        assertThrows(IllegalArgumentException.class, () -> FibonacciCalculator.calculateFibonacciNumbers(-5));
    }
}
