package com.bugsnag.android.unreal;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;
import org.junit.Test;

public class MetadataSerializerTest {

  @Test
  public void testSerializeEmptyMap() throws IOException {
    assertSerializesTo(new HashMap(), "{}");
  }

  @Test
  public void testSerializeNullValue() throws IOException {
    assertSerializesTo(new HashMap() {{
      put("pretzel", null);
    }}, "{\"pretzel\":null}");
  }

  @Test
  public void testSerializeNestedMap() throws IOException {
    assertSerializesTo(new HashMap() {{
      put("todo", new HashMap() {{
        put("fallback", Arrays.asList(new String[] { "samosa", "turnovers" }));
      }});
    }}, "{\"todo\":{\"fallback\":[\"samosa\",\"turnovers\"]}}");
  }

  @Test
  public void testSerializeNumber() throws IOException {
    assertSerializesTo(new HashMap() {{
      put("sou", 3);
    }}, "{\"sou\":3}");
  }

  @Test
  public void testSerializeBoolean() throws IOException {
    assertSerializesTo(new HashMap() {{
      put("carrot cake", true);
    }}, "{\"carrot cake\":true}");
  }

  @Test
  public void testSerializeString() throws IOException {
    assertSerializesTo(new HashMap() {{
        put("expected", "lemon pie");
    }}, "{\"expected\":\"lemon pie\"}");
  }

  @Test(expected=IOException.class)
  public void testSerializeNull() throws IOException {
    MetadataSerializer.serialize(null);
  }

  @Test
  public void testSerializeNonStringKey() throws IOException {
    assertSerializesTo(new HashMap<String, Object>() {{
      put(null, null);
      put("croissant", null);
    }}, "{\"croissant\":null}");
  }

  private void assertSerializesTo(HashMap<String, Object> metadata, String expected) throws IOException {
    assertThat(new String(MetadataSerializer.serialize(metadata)), is(expected));
  }
}
