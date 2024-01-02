import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class LinkedListReversalTest {

    @Test
    void testReverseLinkedList() {
        // Создаем связный список: 1 -> 2 -> 3 -> 4
        Node head = new Node(1);
        head.next = new Node(2);
        head.next.next = new Node(3);
        head.next.next.next = new Node(4);

        // Разворачиваем связный список
        head = LinkedListReversal.reverseLinkedList(head);

        // Проверяем, что развернутый связный список: 4 -> 3 -> 2 -> 1
        assertEquals(4, head.data);
        assertEquals(3, head.next.data);
        assertEquals(2, head.next.next.data);
        assertEquals(1, head.next.next.next.data);
        assertNull(head.next.next.next.next);
    }
}
