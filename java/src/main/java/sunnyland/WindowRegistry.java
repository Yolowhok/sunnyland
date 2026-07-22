package sunnyland;

import java.util.concurrent.ConcurrentHashMap;

/** Аналог glfwSetWindowUserPointer / glfwGetWindowUserPointer */
final class WindowRegistry {
    private static final ConcurrentHashMap<Long, Window> WINDOWS = new ConcurrentHashMap<>();

    private WindowRegistry() {
    }

    static void put(long handle, Window window) {
        WINDOWS.put(handle, window);
    }

    static Window get(long handle) {
        return WINDOWS.get(handle);
    }

    static void remove(long handle) {
        WINDOWS.remove(handle);
    }
}
