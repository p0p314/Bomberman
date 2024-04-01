
class Debouncer {
public:
    Debouncer(float delay);
    bool update(float deltaTime);
    void trigger();
    bool isReady();

private:
    float delay;
    float elapsedTime;
    bool ready;
};
