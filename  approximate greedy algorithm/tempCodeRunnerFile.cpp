
    //Getters and Setters
    void SetNu(int number) { m_number = number; }
    int GetNu() const { return m_number; }
    void SetNa(const string& name) { m_name = name; }
    string GetNa() const { return m_name; }
    void SetSt(const set<State>& states) { m_states = states; }
    const set<State>& GetSt() const { return m_states; 