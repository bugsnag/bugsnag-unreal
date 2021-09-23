package com.bugsnag.android.unreal;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;
import org.junit.Test;

import android.util.Log;

public class MetadataParserTest {

  @Test
  public void testParseEmptyJsonObject() throws IOException {
    assertParsesTo("{}", new HashMap<String, Object>());
  }

  @Test
  public void testParseJsonObjectWithNullValue() throws IOException {
    assertParsesTo("{\"doughnut\":null}", new HashMap<String, Object>() {{
      put("doughnut", null);
    }});
  }

  @Test
  public void testParseNestedJsonObject() throws IOException {
    assertParsesTo(
      "{\"pastries\":{\"apple pie\":2,\"burek\":false},\"counters\":{\"num\":[\"2\",\"44\"]}}",
      new HashMap<String, Object>() {{
        put("pastries", new HashMap<String, Object>() {{
          put("apple pie", 2.0);
          put("burek", false);
        }});
        put("counters", new HashMap<String, Object>() {{
          put("num", Arrays.asList(new String[] { "2", "44" }));
        }});
      }}
    );
  }

  @Test(expected=IOException.class)
  public void testParseNull() throws IOException {
    MetadataParser.parse(null);
  }

  @Test(expected=IOException.class)
  public void testParseJsonArray() throws IOException {
    MetadataParser.parse("[]".getBytes());
  }

  @Test(expected=IOException.class)
  public void testParseValueTooShortToBeJson() throws IOException {
    MetadataParser.parse("{".getBytes());
  }

  @Test(expected=IOException.class)
  public void testParseJsonValue() throws IOException {
    MetadataParser.parse("681".getBytes());
  }

  @Test(expected=IOException.class)
  public void testParseTruncatedJson() throws IOException {
    MetadataParser.parse("{\"key\":[]".getBytes());
  }

  @Test(expected=IOException.class)
  public void testParseInvalidJson() throws IOException {
    MetadataParser.parse("{\"key\"nope".getBytes());
  }

  private void assertParsesTo(String json, HashMap<String, Object>expected) throws IOException {
    assertThat(MetadataParser.parse(json.getBytes()), is(expected));
  }
}
