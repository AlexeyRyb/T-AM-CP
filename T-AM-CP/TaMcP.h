class TaMcP
{
    private:

        int _brakeLeft;
        int _dirLeft;
        int _brakeRight;
        int _dirRight;
        int _speedRight;
        int _speedLeft;
         
        void moveBegin();

    public:

        TaMcP(int pins[6]);

        void moveUp();
        void moveBack();
        void right();
        void left();
        void moveUpRight();
        void moveUpLeft();
        void moveBackRight();
        void moveBackLeft();
        void stopMove();

        
        void setSpd(int speedIn);
        
};

#endif
