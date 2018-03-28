package tests.arithmetic;

import java.lang.reflect.Modifier;


import logic.blocks.Adder;
import logic.blocks.Multiplier;

import java.lang.reflect.Modifier;
import org.junit.Before;
import org.junit.Test;
import org.junit.Assert;


public class BlockTests {

    private Adder adder;
    private Multiplier multiplier;

    @Before
    public void setUp() {
        adder = new Adder(1);
        adder.set_arguments(1,2,3);
        multiplier = new Multiplier(2);
        multiplier.set_arguments(1,2,3,4);
    }

    @Test
    public void test01() {
        Assert.assertEquals("Test id.", 1, adder.id);
        Assert.assertEquals("Test id.", 2, multiplier.id);
    }

    @Test
    public void test02() {
        Assert.assertEquals("Test aritmetiky", 6, adder.execute());
        Assert.assertEquals("Test aritmetiky", 24, multiplier.execute());
    }
}
