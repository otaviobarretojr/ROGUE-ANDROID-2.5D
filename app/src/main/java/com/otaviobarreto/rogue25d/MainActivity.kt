package com.otaviobarreto.rogue25d

import android.app.Activity
import android.content.Intent
import android.graphics.Color
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.widget.Button
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import java.io.File
import java.security.MessageDigest

class MainActivity : Activity() {
    companion object {
        private const val REQUEST_ROM = 221
        private const val ROM_FILE = "emerald_rogue_ex_2_2_1.gba"
        private const val ROM_SIZE = 33_554_432L
        private const val ROM_SHA1 = "7600af1fe08444c850c3c1227fd7dfd81336ae8e"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enterImmersiveMode()

        showIsolationScreen()
    }

    private fun enterImmersiveMode() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_FULLSCREEN or
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    }

    private fun launchRuntime() {
        setContentView(RuntimeSurface(this))
    }

    private fun showIsolationScreen() {
        val romValid = isInstalledRomValid()
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = Gravity.CENTER
            setPadding(72, 72, 72, 72)
            setBackgroundColor(Color.rgb(18, 34, 28))
        }
        val title = TextView(this).apply {
            text = "ROGUE 2.5D — Android Safe Start"
            textSize = 30f
            setTextColor(Color.WHITE)
            gravity = Gravity.CENTER
        }
        val status = TextView(this).apply {
            text = if (romValid)
                "Android/Kotlin iniciou corretamente. ROM EX v2.2.1 encontrada."
            else
                "Android/Kotlin iniciou corretamente. ROM ainda não instalada."
            textSize = 18f
            setTextColor(Color.WHITE)
            gravity = Gravity.CENTER
            setPadding(0, 28, 0, 32)
        }
        val nativeTest = Button(this).apply {
            text = "Testar runtime nativo"
            isEnabled = romValid
            setOnClickListener { launchRuntime() }
        }
        val romButton = Button(this).apply {
            text = if (romValid) "Selecionar outra ROM" else "Selecionar ROM .gba"
            setOnClickListener { requestRom() }
        }
        root.addView(title)
        root.addView(status)
        root.addView(nativeTest)
        root.addView(romButton)
        setContentView(root)
    }


    private fun showRomImportScreen() {
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = Gravity.CENTER
            setPadding(72, 72, 72, 72)
            setBackgroundColor(Color.rgb(18, 34, 28))
        }

        val title = TextView(this).apply {
            text = "ROGUE 2.5D"
            textSize = 32f
            setTextColor(Color.WHITE)
            gravity = Gravity.CENTER
        }

        val body = TextView(this).apply {
            text = "Selecione sua ROM Pokémon Emerald Rogue EX v2.2.1.\n\n" +
                "A ROM não é incluída no APK. Ela é usada apenas no armazenamento privado do aplicativo para carregar os dados originais do jogo."
            textSize = 18f
            setTextColor(Color.WHITE)
            gravity = Gravity.CENTER
            setPadding(0, 28, 0, 36)
        }

        val select = Button(this).apply {
            text = "Selecionar ROM .gba"
            setOnClickListener { requestRom() }
        }

        root.addView(title)
        root.addView(body)
        root.addView(select)
        setContentView(root)
    }

    private fun requestRom() {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "application/octet-stream"
            putExtra(Intent.EXTRA_MIME_TYPES, arrayOf(
                "application/octet-stream",
                "application/x-gba-rom",
                "application/x-gameboy-advance-rom"
            ))
        }
        startActivityForResult(intent, REQUEST_ROM)
    }

    @Deprecated("Legacy result callback retained for API 26 compatibility")
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode != REQUEST_ROM || resultCode != RESULT_OK) return

        val uri = data?.data ?: return
        val temp = File(filesDir, "$ROM_FILE.tmp")
        val target = File(filesDir, ROM_FILE)

        try {
            val digest = MessageDigest.getInstance("SHA-1")
            var total = 0L

            contentResolver.openInputStream(uri).use { input ->
                requireNotNull(input) { "Não foi possível abrir o arquivo." }
                temp.outputStream().use { output ->
                    val buffer = ByteArray(64 * 1024)
                    while (true) {
                        val read = input.read(buffer)
                        if (read < 0) break
                        if (read == 0) continue
                        digest.update(buffer, 0, read)
                        output.write(buffer, 0, read)
                        total += read
                    }
                    output.flush()
                }
            }

            val sha1 = digest.digest().joinToString("") { "%02x".format(it) }
            if (total != ROM_SIZE || sha1 != ROM_SHA1) {
                temp.delete()
                Toast.makeText(
                    this,
                    "ROM incompatível. É necessária exatamente a versão EX v2.2.1.",
                    Toast.LENGTH_LONG
                ).show()
                return
            }

            if (target.exists() && !target.delete()) {
                throw IllegalStateException("Não foi possível substituir a ROM anterior.")
            }
            if (!temp.renameTo(target)) {
                temp.copyTo(target, overwrite = true)
                temp.delete()
            }

            showIsolationScreen()
        } catch (t: Throwable) {
            temp.delete()
            Toast.makeText(this, "Falha ao importar ROM: ${t.message}", Toast.LENGTH_LONG).show()
        }
    }

    private fun isInstalledRomValid(): Boolean {
        val rom = File(filesDir, ROM_FILE)
        if (!rom.isFile || rom.length() != ROM_SIZE) return false

        return try {
            val digest = MessageDigest.getInstance("SHA-1")
            rom.inputStream().use { input ->
                val buffer = ByteArray(64 * 1024)
                while (true) {
                    val read = input.read(buffer)
                    if (read < 0) break
                    if (read > 0) digest.update(buffer, 0, read)
                }
            }
            digest.digest().joinToString("") { "%02x".format(it) } == ROM_SHA1
        } catch (_: Throwable) {
            false
        }
    }
}
