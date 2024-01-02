import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class PalindromeCheckerTest {

    @Test
    void testIsPalindrome() {
        assertTrue(PalindromeChecker.isPalindrome(121));
        assertTrue(PalindromeChecker.isPalindrome(1221));
        assertTrue(PalindromeChecker.isPalindrome(12321));

        assertFalse(PalindromeChecker.isPalindrome(123));
        assertFalse(PalindromeChecker.isPalindrome(1234));
    }
}
