package com.moonkey.cipher

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalClipboardManager
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.AnnotatedString
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.moonkey.cipher.ui.theme.InputFieldBg
import com.moonkey.cipher.ui.theme.MyAppTheme
import androidx.compose.animation.AnimatedContent
import androidx.compose.animation.core.tween
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.togetherWith


class MainActivity : ComponentActivity() {
    companion object {
        init { System.loadLibrary("cipher") }
    }

    private external fun cipherNative(text1: String, text2: String): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MyAppTheme {
                InputConcatScreen { t1, t2 -> cipherNative(t1, t2) }
            }
        }
    }
}

@Composable
fun InputConcatScreen(cipher: (String, String) -> String) {
    var input1 by remember { mutableStateOf("") }
    var input2 by remember { mutableStateOf("") }
    val canGenerate by remember(input1, input2) {
        derivedStateOf { input1.isNotBlank() && input2.isNotBlank() }
    }
    val concatenated = remember(input1, input2) {
        if (canGenerate) cipher(input1, input2) else ""
    }
    val clipboard = LocalClipboardManager.current
    val context = LocalContext.current

    Box(
        modifier = Modifier
            .fillMaxSize()
            .background(MaterialTheme.colorScheme.background)
            .imePadding(),
        contentAlignment = Alignment.Center
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            modifier = Modifier
                .padding(24.dp)
                .fillMaxWidth(0.8f)
        ) {
            Text(
                text = "Cipher",
                style = MaterialTheme.typography.displayLarge,
                modifier = Modifier.padding(bottom = 24.dp)
            )
            Card(
                shape = RoundedCornerShape(16.dp),
                modifier = Modifier
                    .fillMaxWidth()
                    .background(InputFieldBg)
                    .padding(vertical = 8.dp)
            ) {
                TextField(
                    value = input1,
                    onValueChange = { input1 = it },
                    label = { Text("请在这里输入salt") },
                    modifier = Modifier.fillMaxWidth()
                )
            }
            Card(
                shape = RoundedCornerShape(16.dp),
                modifier = Modifier
                    .fillMaxWidth()
                    .background(InputFieldBg)
                    .padding(vertical = 8.dp)
            ) {
                TextField(
                    value = input2,
                    onValueChange = { input2 = it },
                    label = { Text("请在这里输入input") },
                    modifier = Modifier.fillMaxWidth()
                )
            }
            Spacer(modifier = Modifier.height(16.dp))
            Button(
                onClick = {
                    if (canGenerate && !cipher(input1, input2).contains("invalid" )) {
                        clipboard.setText(AnnotatedString(concatenated))
                        Toast.makeText(context, "已复制", Toast.LENGTH_SHORT).show()
                    } else {
                        Toast.makeText(context, "等待输入", Toast.LENGTH_SHORT).show()
                    }
                },
                modifier = Modifier
                    .fillMaxWidth()
                    .height(48.dp),
                shape = RoundedCornerShape(24.dp)
            ) {
                AnimatedContent(
                    targetState = if (canGenerate) concatenated else "等待输入",
                    transitionSpec = {
                        fadeIn(animationSpec = tween(300)) togetherWith fadeOut(animationSpec = tween(300))
                    }
                ) { result ->
                    Text(
                        text = result.ifEmpty { "点击复制" },
                        fontSize = 16.sp
                    )
                }
            }

        }
    }
}
