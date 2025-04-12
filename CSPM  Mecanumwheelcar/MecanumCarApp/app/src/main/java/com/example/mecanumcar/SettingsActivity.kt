package com.example.mecanumcar

import android.os.Bundle
import android.text.InputFilter
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

class SettingsActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_settings)

        val ipField1 = findViewById<EditText>(R.id.ipField1)
        val ipField2 = findViewById<EditText>(R.id.ipField2)
        val ipField3 = findViewById<EditText>(R.id.ipField3)
        val ipField4 = findViewById<EditText>(R.id.ipField4)
        val portField = findViewById<EditText>(R.id.portField)
        val statusPortField = findViewById<EditText>(R.id.statusPortField)
        val saveButton = findViewById<Button>(R.id.saveButton)
        val exitButton = findViewById<Button>(R.id.exitButton)

        // Set input filters
        val ipFilter = InputFilter { source, _, _, _, _, _ ->
            try {
                val input = source.toString().toInt()
                if (input in 0..255) source else ""
            } catch (e: NumberFormatException) {
                ""
            }
        }
        ipField1.filters = arrayOf(ipFilter)
        ipField2.filters = arrayOf(ipFilter)
        ipField3.filters = arrayOf(ipFilter)
        ipField4.filters = arrayOf(ipFilter)

        val portFilter = InputFilter { source, _, _, _, _, _ ->
            try {
                val input = source.toString().toInt()
                if (input in 1025..65535) source else ""
            } catch (e: NumberFormatException) {
                ""
            }
        }
        portField.filters = arrayOf(portFilter)
        statusPortField.filters = arrayOf(portFilter)

        // Load existing settings
        val sharedPrefs = getSharedPreferences("MecanumCarSettings", MODE_PRIVATE)
        ipField1.setText(sharedPrefs.getString("carIpField1", "192"))
        ipField2.setText(sharedPrefs.getString("carIpField2", "168"))
        ipField3.setText(sharedPrefs.getString("carIpField3", "10"))
        ipField4.setText(sharedPrefs.getString("carIpField4", "1"))
        portField.setText(sharedPrefs.getString("carPort", "4444"))
        statusPortField.setText(sharedPrefs.getString("statusPort", "4445"))

        // Save settings on button click
        saveButton.setOnClickListener {
            val newIp = "${ipField1.text}.${ipField2.text}.${ipField3.text}.${ipField4.text}"
            val newPort = portField.text.toString()
            val newStatusPort = statusPortField.text.toString()

            if (ipField1.text.isBlank() || ipField2.text.isBlank() || ipField3.text.isBlank() || ipField4.text.isBlank() ||
                newPort.isBlank() || newStatusPort.isBlank()) {
                Toast.makeText(this, "Please fill in all fields", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            val editor = sharedPrefs.edit()
            editor.putString("carIpField1", ipField1.text.toString())
            editor.putString("carIpField2", ipField2.text.toString())
            editor.putString("carIpField3", ipField3.text.toString())
            editor.putString("carIpField4", ipField4.text.toString())
            editor.putString("carIpAddress", newIp)
            editor.putString("carPort", newPort)
            editor.putString("statusPort", newStatusPort)
            editor.apply()

            Toast.makeText(this, "Settings saved", Toast.LENGTH_SHORT).show()
            finish()
        }
        // Exit without saving on button click
        exitButton.setOnClickListener {
            finish()
        }
    }
}
