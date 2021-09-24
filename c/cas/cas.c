bool compare_and_swap(int *reg,int oldval,int newval)
{
    int reg_val = *reg;
    if(reg_val == oldval)
    {
        *reg = newval;
        return true;
    }
    return false;
}
